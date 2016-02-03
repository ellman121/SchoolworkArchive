#|
		***** genealogy.lsp *****

This lisp program allows for the user to query a database 
of person structures.

Usage:
		clisp -repl genealogy.lsp <family_database_file>

Queries: parents, mother, father, children, sons, daughetrs,
siblings, brothers, sisters, cousins, male-cousins, female-cousins,
uncles, aunts, grandparents, grandfathers, grandmothers, grandchildren,
male-grandchildren, female-grandchildren, ancestors, male-ancestors,
female-ancestors, descendents, male-descendents, female-descendents

Author: Elliott Rarden
Class:  CSC461 - Programming Languages
Date:   Fall 2015

|#


;;;; DEFSTRUCT PERSON
;;; This struct defines a person entry in the database
;;; Includes constructors (make-person), (person-name)
;;; (person-gender), (person-parents), (person-children)
(defstruct person
	name 		; Name is a string
	gender 		; Gender is a string - "male" or "female"
	parents 	; Parents is a list of names
	children) 	; Children is a list of names

;;;; DEFVAR *DATABASE*
;;; This is a global database variable used to hold the persons
;;; It's just a list of PERSON structs, easy to walk through
(defvar *database*)

;;;; FUNCTION FLATTEN (lst)
;;; Given a list with sublists, this function returns a list
;;; such that all sublist elements have been moved into the top 
;;; level.  e.g. ((1, 2), 3, ((4, 5), 6)) --> (1, 2, 3, 4, 5, 6)
(defun flatten (lst)
	"Flatten a list (move all elements of sublists into the top level list)"
	(cond ((null lst) nil)
		((atom lst) (list lst))
		(t (loop for all in lst appending (flatten all)))
	)
)

;;;; FUNCTION lst-to-person

(defun lst-to-person (lst)
	"Convert a list with a person's attributes to a person object"
	(make-person
		:name (car lst)
		:gender (car (cdr lst))
		:parents (car (cdr (cdr lst)))
		:children (car (cdr (cdr (cdr lst))))
	)
)

(defun read-file-into-db (filename)
	"Return a list where each entry is a list of each line"
	(with-open-file (stream filename)
		(loop for line = (read-line stream nil)
			while line
			collect (lst-to-person (read-from-string line))
		)
	)
)

(defun list-db-by-line ()
	"List each person in the db, one on each line"
	(loop for prsn in *database*
		do (print prsn)
	)
)

(defun get-person-at-index (indx &optional (data *database*))
	"Return the person at the specified index (return nil if out of bounds)"
	(cond
		((or (>= indx (length *database*)) (< indx 0))
			; If we are out of bounds, return nil
			nil)
		((eq indx 0)
			; If index is 0, return the value
			(car data))
		((> indx 0)
			; If index is not 0, check the next value in the list
			(get-person-at-index (- indx 1) (cdr data)))
	)
)

(defun get-person-by-name (name)
	"Given a person's name (as a string),
	return the first person object found in the databse with that name"
	(loop for x in *database*
		do (if (string-equal (person-name x) name)
			(return-from get-person-by-name x) ; True - return the person
			() ; False - do nothing
		)
	)
)

(defun malefilter (lst)
	; This let statement converts the list of names provided into a list of person objects
	; and initializes ret to the empty list
	(let ((pobjs (loop for name in lst collect (get-person-by-name name))) (ret '()))
		(remove nil (loop for prsn in pobjs ; Loop over the person objects
			collect (if (string-equal (person-gender prsn) "male") (person-name prsn) ())
		))
	)
)


(defun femalefilter (lst)
	; This let statement converts the list of names provided into a list of person objects
	; and initializes ret to the empty list
	(let ((pobjs (loop for name in lst collect (get-person-by-name name))) (ret '()))
		(remove nil (loop for prsn in pobjs ; Loop over the person objects
			collect (if (string-equal (person-gender prsn) "female") (person-name prsn) ())
		))
	)
)

(defun parents (p)
	"Given a person's name (as a string),
	return a list of the parent's names (as strings)"
	(let (ret = '())
		(loop for prsn in *database*
			do (if (string-equal (person-name prsn) p)
				(let ((lst (person-parents prsn))) ; If we found the person in the db, continue
					(if (null lst)
						() ; If person has no parents on record, move along
						(loop for name in lst ; else, add parent's names to return list
							do (pushnew name ret) ; You can't have a parent twice, so ignore duplicate
						)
					)
				)
			)
		)
		(sort ret #'string-lessp)
	)
)

(defun mothers (p)
	(femalefilter (parents p))
)

(defun fathers (p)
	(malefilter (parents p))
)

(defun children (p)
	"Given a person's name (as a string),
	return a list of the children's names (as strings)"
	(let (ret = '())
		(loop for prsn in *database*
			do (if (string-equal (person-name prsn) p)
				(let ((lst (person-children prsn))) ; If we found the person in the db, continue
					(if (null lst)
						() ; If person has no parents on record, move along
						(loop for name in lst ; else, add parent's names to return list
							do (push name ret)
						)
					)
				)
			)
		)
		ret
	)
)

(defun sons (p)
	(malefilter (children p))
)

(defun daughters (p)
	(femalefilter (children p))
)

(defun siblings (p)
	(let ((ret '()))
		(loop for par in (parents p)
			do (loop for child in (children par)
				do (pushnew child ret
				)
			)
		)
		(remove nil (remove p ret))
	)
)

(defun sisters (p)
	(femalefilter (siblings p))
)

(defun brothers (p)
	(malefilter (siblings p))
)

(defun grandparents (p)
	(let ((ret '()))
		(loop for par in (parents p)
			do (loop for gpar in (parents par)
				do (pushnew gpar ret)
			)
		)
		(flatten ret)
	)
)

(defun grandfathers (p)
	(malefilter (grandparents p))
)

(defun grandmothers (p)
	(femalefilter (grandparents p))
)

(defun grandchildren (p)
	(let ((ret '()))
		(loop for child in (children p)
			do (loop for gchild in (children child)
				do (pushnew gchild ret)
			)
		)
	(sort ret #'string-lessp)
	)
)

(defun grandsons (p)
	(malefilter (grandchildren p))
)

(defun granddaughters (p)
	(femalefilter (grandchildren p))
)

(defun uncles (p)
	(let ((ret '()))
		(loop for par in (parents p)
			do (loop for sib in (siblings par)
				do (if (string-equal (person-gender (get-person-by-name sib)) "male")
					(pushnew sib ret)
					()
				)
			)
		)
		(remove nil ret)
	)
)

(defun aunts (p)
	(let ((ret '()))
		(loop for par in (parents p)
			do (loop for sib in (siblings par)
				do (if (string-equal (person-gender (get-person-by-name sib)) "female")
					(pushnew sib ret)
					()
				)
			)
		)
		(remove nil ret)
	)
)

(defun nieces (p)
	(let ((ret '()))
		(loop for sib in (siblings p)
			do (loop for child in (children sib)
				do (if (string-equal (person-gender (get-person-by-name child)) "female")
					(pushnew child ret)
					()
				)
			)
		)
		(remove nil ret)
	)
)

(defun nephews (p)
	(let ((ret '()))
		(loop for sib in (siblings p)
			do (loop for child in (children sib)
				do (if (string-equal (person-gender (get-person-by-name child)) "male")
					(pushnew child ret)
					()
				)
			)
		)
		(remove nil ret)
	)
)

(defun cousins (p)
	(let ((ret '()))
		(loop for par in (parents p)
			do (loop for sib in (siblings par)
				do (loop for child in (children sib)
					do (pushnew child ret)
				)
			)
		)
		(remove nil ret)
	)
)

(defun male-cousins (p)
	(malefilter (cousins p))
)

(defun female-cousins (p)
	(femalefilter (cousins p))
)

(defun ancestors (p)
	(let ((ret (parents p)))
		(flatten (append ret (remove nil (loop for par in (parents p) collect (ancestors par)))))
	)
)

(defun male-ancestors (p)
	(malefilter (ancestors p))
)

(defun female-ancestors (p)
	(femalefilter (ancestors p))
)

(defun descendents (p)
	(let ((ret (children p)))
		(flatten (append ret (remove nil (loop for child in (children p) collect (descendents child)))))
	)
)

(defun male-descendents (p)
	(malefilter (descendents p))
)

(defun female-descendents (p)
	(femalefilter (descendents p))
)

(setf *database* (read-file-into-db (car (last *ARGS*))))
