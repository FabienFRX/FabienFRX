(define (apply-twice f x)
  ((lambda (y) (f (f y))) x))

(define (inc x) (+ x 1))
(apply-twice inc 5)