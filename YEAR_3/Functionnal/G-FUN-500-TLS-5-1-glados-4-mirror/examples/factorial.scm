(define (fact n)
  (if (eq? n 0)
    1
    (* n (fact (- n 1)))))
(fact 5)
(fact 10)