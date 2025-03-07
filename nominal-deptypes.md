# Nominal Intensional Dependent Type Checking

## Dep-what?

Programming languages which are statically typed rely on a type system to express
the notion of correctness. A type-checking algorithm will inspect a program in
that language, model its properties using the type system, and if the model 
obeys the rules of the type system, the algorithm will report that the program is
"type safe".

The type system is normally a separate language from the actual programming 
language. The programming language is called the "term" language and the type
system language is called the "type language". The reason these two languages are
separate is because the type language is normally a very simple language that 
isn't as expressive as the term language. This means the type language can't 
express as many things as the term language, but the things it can express are
guaranteed to follow the rules and be "type safe".

Dependent type system does not use a separate language, it uses the same language
for both terms and types. This means that the type-level model of a program, is 
the program itself. If you ask a mathemetician, they'll explain this using 
dependent sums and dependent products. It's non-obvious how these have a 
computational interpretation, so I'm not going to use them. An easier way to put
it is using the Lambda Cube. A full dependently typed language is classified as a
"higher order dependently typed polymorphic lambda calculus". In other words, you
can use the same language to create a function from term to term, term to type, 
type to term, and type to type.

Now, in order to make an assertion using this dependently typed language, you 
would simply write a function.\
`inc x = x + 1`\
What would the type of this function be? Simple:\
`lambda x . x + 1`

Here's where things become complicated.
Lets say I had another function,\
`add1 x = x + 2 - 1`\
and I wanted to assert \
`inc : add1`\
Now it gets tricky because `add1` is not a reduced type-level language, it's the 
whole term-level language.

Here's a new word, Extensionality. It's the property that two functions which 
might have different constructions, still share the same image and preimage. For
total functions with finite, countable images and preimages, you can decide if 
two functions are extensional (given enough time). Partial functions are a 
different story. Now you have to grapple with the halting problem. If a function 
diverges, you will never be able to decide if its image and preimage are the same
as some other function, because you will never be able to determine all the 
members. You will only have a *partial* image/preimage to work with. We got lucky
with `add1` and `inc` above because they're trivially total and it's easy to see
that they do the same thing just by reading them. In the real world, it's usually
much more complicated.

This is the great big snag with dependently typed languages. And probably the 
biggest reason why they haven't caught on. If you want to use the same language 
in both the term and type level, you have to incorporate formal mathematical 
proofs into your program so that `infiniteloopA : infiniteloopB` might actually 
typecheck before the heat death of the universe.

There's two issues with this. 
1) Good luck creating a formal mathematical proof for every little bit of your 
program. Often times there's no proof that you could *ever* construct which will
properly verify your program.
2) Formal mathematical proofs are very poor at telling you if you've met product 
design's requirement that the top left button should be purple on every second 
Tuesday.

My solution to both these problems is to flip dependent type checking upside 
down. Mathemeticians might look at `f x` and ask, "Is this the same as evaluating 
`g x`?". A programmer/computer scientist would instead ask, "How can I predict the
effects of evaluating this?". Only the first question is answered by extensionality.
The latter is a different question entirely. Coincidentally this is also the 
question that most people are asking when they turn to dependent types for more 
control over type checking granularity.
