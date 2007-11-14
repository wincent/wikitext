The files in this directory are not part of the wikitext extension itself;
rather, they relate to one of its dependencies, ANTLR.

I wanted to keep a copy of the ANTLR source in this wikitext repository for
the following reasons:

  - ANTLR is absolutely critical to the wikitext extension in its current
    form (see note below); keeping a copy of the source on hand serves as an
    insurance policy in case the official ANTLR site ever goes away.
  - previous ANTLR updates (undocumented API changes) have caused breakage in
    the wikitext extension; keeping a known-good version of the source shields
    against possible future breakage.
  - there is no publicly accessible source code repository for ANTLR, so
    maintaining patches against it can be somewhat painful; keeping a pristine
    copy of the source along with patches against it and scripts to automate
    patching and building remove some of the pain.

[Note: It's possible that in the future I might replace this dependency on ANTLR with
a custom lexer written using Ragel (depending on Ragel is somewhat simpler as
it doesn't require the cumbersome Java-based toolchain).]

For more information about ANTLR, see:

  <http://antlr.org/>
