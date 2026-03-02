((lambda (x) (* x x)) 5)

(define add
  (lambda (a b)
    (+ a b)))
(add 10 20)

((lambda (x y) (+ (* x x) (* y y))) 3 4)