(if #t 1 2)
(if #f 1 2)

(define x 10)
(if (< x 5)
  (* x 2)
  (+ x 1))