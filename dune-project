(lang dune 3.8)
(name bastet)
(source (github Risto-Stevcev/bastet))
(license BSD-3-Clause)
(authors "Risto Stevcev")
(maintainers "me@risto.codes")
(generate_opam_files false) ;disable while pinning melange deps
(implicit_transitive_deps false)
(using melange 0.1)
(using mdx 0.3)

(package
 (name bastet)
 (synopsis "An OCaml library for category theory and abstract algebra")
 (documentation "https://risto-stevcev.github.io/bastet")
 (tags
  ("category theory" "abstract algebra" "algebra" "cats"))
 (depends
  (ocaml (>= 4.08))
  (alcotest (and (>= 1.0.1) :with-test))
  (qcheck (and (and (>= 0.13) (< 0.18)) :with-test))
  (qcheck-alcotest (and (and (>= 0.13) (< 0.18)) :with-test))
  (mdx (and (>= 1.6.0) :with-test))
  (bisect_ppx (and (>= 2.1.0)))
  (odoc (and (>= 1.5.0) :with-doc))
  (mustache (and (>= 3.1.0) :with-doc))
  (dune (>= 2.0.1))))
