# Lisp Stuff

So today in Maths Club, we discussed complex numbers. However, we've already done this a few times last year, and they were covered in class as well, so by now it was pretty routine. Add to that that I haven't had enough sleep last night, and so have had difficulty concentrating all day, and I did not in fact make notes about complex numbers during Maths Club.

Instead, I was working on my lisp#rholisp implementation. Specifically, I wanted to add block comments, for I am in the middle of implementing a program that uses its FFi, and at times when my program crashed it was very useful to comment out large blocks to find where my program still worked – something made rather tedious by my language only having line comments.

As I was exploring this, I found a few different approaches. CommonLisp, apparently, uses paired `#|` and `|#` for block comments, allowing nesting. One feature of some lisps – and I cannot find a reference to it again, search engines are quite useless at finding strings of symbols unfortunately – is that something along the lines of `#_` would skip the next S-expr at the reader level (if I understand correctly). Now, while I wanted block comments, I disliked the idea of having to implement new syntax for them, as I want to try to keep the actual language implementation relatively small. One simple solution presented itself: I could define a macro `(defm #_ (() ...) ())`, which would discard all of its (unevaluated) arguments, returning nil.

Exploring this, however, lead to a revelation: apparently, I wasn't doing macros the normal lisp style. Well, that's not too surprising, but how it actually works is interesting. Apparently, there is an extra step in evaluation of most lisps which I never implemented. Where I just parse the expressions, then evaluate them straight away, it seems that other lisps expand macros _before_ evaluating functions.

So, in rholisp, macros are treated similar to functions by evaluation. Instead of creating a new environment with its arguments and evaluating its body, though, a macro instead takes its body and places its arguments inside the body as-is (unevaluated) wherever a parameter name can be found. This substituted body, is then evaluated in place of the macro.

The idea I get of a more typical lisp, is that first a macro's arguments is macro-expanded, and then the macro itself is expanded (rather than taking literal arguments)^. To expand a macro, it actually runs as any normal function would, though its arguments are the source as given (albeit expanded), rather than the result of evaluating expressions. Then, the result of this function is evaluated.

^ I'm unsure about this, this seems rather strange to me...

I must say, this does seem more powerful than my approach, but after a few minutes of thinking, I think it could be implemented in terms of my macros (other than premature macro expansion of arguments):

```
(defm defmacro (name args body)
  (def name (\* (().)
    (eval (call (\ args body) .))
  ))
)
```

Then, something like the following:

```
; adapted from https://lisp-lang.org/learn/macros
; just ignore for now that (loop) doesn't exist in rholisp...
; or that ` and , don't exist either, for that matter
(defmacro while (condition () body)
  `(loop while , condition do ,(cons ' do body))
)
```

Would expand like so:

```
(def while (\* (().)
  (eval (call (\ (condition () body)
    `(loop while , condition do ,(cons ' do body))
  ) ' .))
))
```

Then, if we call it, the following would happen:

```
(while (cond) (do-thing-1) (do-thing-2))
; its arguments list gets bound to `.`,
; which is literally replaced in the macro body,
; expanding to:
(eval (call (\ (condition () body)
  `(loop while , condition do ,(cons ' do body))
) '((cond) (do-thing-1) (do-thing-2)) ))
; now (cond) is bound to the condition parameter,
; and ((do-thing-1) (do-thing-2)) to body,
; so calling the function results in something equivalent to:
(eval '(loop while (cond) do (do (do-thing-1) (do-thing-2))) )
; which will now evaluate to the same as the following code:
(loop while (cond) do (do (do-thing-1) (do-thing-2)))
```

So a surface-level evaluation would seem to suggest that my macro scheme is as powerful as the usual lisp macros.

Though in the above example, I skimmed over something quite significant: The use of quasi-quoting. This is not a feature I currently have, but one I've noted a few times would be quite useful.

> To summarise: a single quote followed by an expression is expanded to `(quote <expr>)` _before_ evaluation, where `quote` just returns the expression unchanged. Quasi-quote works similarly, in that a backtick followed by an expression will expand to `(quasiquote <expr>)`, with one major difference: a comma preceding an expression will expand to `(unquote <expr>)`, and when `unquote` is encountered inside of a quasi-quoted expression its argument will be evaluated, rather than `(unquote <expr>)` being left untouched as it would be within a `quote`.
>
> This removes the need for constructing a list with the majority of its members quoted – for example, `´(loop while , condition do ,(cons ' do body))` without quasi-quoting would instead be `(list ' loop ' while condition ' do (cons ' do body))`.

In theory, while the syntax sugar to be able to use `´` and `,` as a shorthand for `(quasiquote ...)` and `(unquote ...)` would need language support at the parsing level, I should be able to implement the actual macros themselves at a standard library level without language support. Something like the following:

```
(defm quasiquote (() args)
  (tmpfn (.args)
    (if .args
      (assoc (val (head args))
        (if (and (= (type val) list) val (= (head val) ' unquote))
          (cons
            (eval (scd val))
            (rec (tail args))
          )
          (cons
            val
            (rec (tail args))
          )
        )
      )
    )
    (args)
  )
)
(defn unquote (expr)
  (assert (and F "unquote should only appear within (quasiquote)"))
)
```

Another thing I found out is that apparently in lisp the fundamental decision structure is `cond`, rather than `if`, and indeed it seems much more idiomatic in many situations. So I also plan on migrating rholisp over from having builtin `if` to having builtin `cond` and having `if` just as a macro.

[random rambling over here over why I don't already know all this stuff, my background with lisp, etc]
