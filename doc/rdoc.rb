def notes #:nodoc: all
  # This file is a terrible hack to get around RDoc's problems parsing C
  # source files.
  #
  # The first problem is that it doesn't allow you to place documentation
  # in header files; it has to be in the implementation file. While this
  # is fine for Ruby code (which doesn't have headers) it's not really
  # appropriate for C and the C "ethos".
  #
  # The second problem is that it can't handle documentation split across
  # multiple source files. I tried to get around this by sticking all of
  # the documentation in one file and hiding it inside a couple of
  # preprocessor directives, but RDoc's parser is too brittle to cope
  # with that; getting the documentation to appear next to the
  # corresponding methods was a delicate balancing act that was too
  # fiddly and cumbersome to be maintainable in the long run.
  #
  # This file is the somewhat ugly solution: we set up a phoney Ruby file
  # and write all our documentation in that; RDoc can parse Ruby code
  # adequately, it would seem.
end

# The Wikitext module provides a namespace for all of the extension.
# In practice, all your interaction will be with the Wikitext::Parser
# class.
module Wikitext

  # = Attributes
  #
  # == +line_ending+ (String)
  #
  # The line ending to be used in the generated HTML (defaults to "\n").
  #
  # == +internal_link_prefix+ (String)
  #
  # The prefix to be prepended to internal links (defaults to "/wiki/").
  # For example, given an internal_link_prefix of "/wiki/", the internal
  # link:
  #     [[Apple]]
  # would be transformed into:
  #     <a href="/wiki/Apple">Apple</a>
  #
  # == +external_link_class+ (String)
  #
  # The CSS class to be applied to external links (defaults to "external").
  # For example, given an external_link_class of "external", the external
  # link:
  #     [http://www.google.com/ the best search engine]
  # would be transformed into:
  #     <a class="external" href="http://www.google.com/">the best search engine</a>
  #
  # == +mailto_class+ (String)
  #
  # The CSS class to be applied to external "mailto" links (defaults to
  # "mailto"). For example:
  #     [mailto:user@example.com user@example.com]
  # or if autolinking of email addresses is turned on, just:
  #     user@example.com
  # would be transformed into:
  #     <a class="mailto" href="mailto:user@example.com">user@example.com</a>
  #
  # == +img_prefix+ (String)
  #
  # The prefix to be prepended to image tags (defaults to "/images/").
  # For example, given this image markup:
  #     {{foo.png}}
  # The following +img+ tag would be produced:
  #     <img src="/images/foo.png" alt="foo.png" />
  #
  # == +autolink+ (boolean)
  #
  # Whether to autolink URIs found in the plain scope.
  # When true:
  #     http://apple.com/
  # will be transformed to:
  #     <a href="http://apple.com/">http://apple.com/</a>
  # and if an external_link_class is set (to "external", for example) then
  # the transformation will be:
  #     <a class="external" href="http://apple.com/">http://apple.com/</a>
  # When false, no transformation will be applied and the link will be
  # echoed literally:
  #     http://apple.com/
  #
  # == +treat_slash_as_special+ (boolean)
  #
  # Whether "slash" in link text is treated specially. When true, any link
  # containing a slash and matching <tt>\A[a-z]+\/\d+\z</tt> is considered to
  # be a relative link _within_ the current site, but _outside_ the wiki. In
  # other words, while:
  #     [[interesting article]]
  # is a wiki link (assuming the internal_link_prefix of "/wiki/"):
  #     <a href="/wiki/interesting+article">interesting article</a>
  # in contrast:
  #     [[issue/400]]
  # is interpreted as a link external to the wiki but internal to the site, and is converted into:
  #     <a href="/issue/400">issue/400</a>
  # this design is intended to work well with preprocessors, that can scan the input for things like:
  #     issue #400
  # and transform them before feeding them into the wikitext parser as:
  #     [[issue/400|issue #400]]
  # which in turn would be transformed into:
  #     <a href="/issue/400">issue #400</a>
  #
  # == +space_to_underscore+ (boolean)
  #
  # Whether spaces in link targets should be encoded normally or transformed
  # into underscores.
  #
  # When false (the default), an internal link like:
  #   [[foo bar]]
  # Would be converted into:
  #   <a href="/wiki/foo%20bar">foo bar</a>
  # But when true, it would be converted into:
  #   <a href="/wiki/foo_bar">foo bar</a>
  #
  # Converting spaces to underscores makes most URLs prettier, but it comes at
  # a cost: when this mode is true the articles "foo bar" and "foo_bar" can no
  # longer be disambiguated, and a link to "foo_bar" will actually resolve to
  # "foo bar"; it is therefore recommended that you explicitly disallow
  # underscores in titles at the application level so as to avoid this kind of
  # confusion.
  class Parser

    # Sanitizes an internal link target for inclusion within the HTML
    # stream. Expects +string+ to be UTF-8-encoded.
    #
    # For example, a link target for the article titled:
    #     foo, "bar" & baz €
    # would be sanitized as:
    #     foo, &quot;bar&quot; &amp; baz &#x20ac;
    # Note that characters which have special meaning within HTML such as
    # quotes and ampersands are turned into named entities, and characters
    # outside of the printable ASCII range are turned into hexadecimal
    # entities.
    #
    # See also encode_link_target.
    def self.sanitize_link_target string
      # This is just a placeholder.
      # See parser.c for the C source code to this method.
    end

    # URL-encodes an internal link target for use as an href attribute in an
    # anchor. Expects +string+ to be UTF-8-encoded.
    #
    # For example, the link target:
    #     foo, "bar" & baz €
    # would be encoded as:
    #     foo%2c%20%22bar%22%20%26%20baz%e2%82%ac
    # The encoding is based on RFCs 2396 and 2718. The "unreserved" characters
    # a..z, a..Z, 0..9, "-", "_", "." and "~" are passed through unchanged and
    # all others are converted into percent escapes.
    #
    # When combined with sanitize_link_target this method can be used to emit
    # the following link for the example article:
    #     <a href="foo%2c%20%22bar%22%20%26%20baz%e2%82%ac">foo, &quot;bar&quot; &amp; baz &#x20ac;</a>
    def self.encode_link_target string
      # This is just a placeholder.
      # See parser.c for the C source code to this method.
    end

    # Prepares a Parser instance.
    #
    # There are a number of attributes that you can set on the returned
    # parser to customize its behaviour. See the attributes documentation
    # in the Parser class. You also have the option of overriding the
    # attributes at initialization time passing in the attribute name in
    # symbol form together with the overridden value.
    #
    # In other words, both:
    #     parser = Wikitext::Parser.new
    #     parser.autolink = false
    #     parser.mailto_class = 'mail'
    # And:
    #     parser = Wikitext::Parser.new(:autolink => false, :mailto_class => 'mail')
    # Are equivalent.
    def initialize options = {}
      # This is just a placeholder.
      # See parser.c for the C source code to this method.
    end

    # Feeds the UTF-8-encoded +string+ into the scanner and returns an
    # array of recognized tokens. Raises a Wikitext::Parser::Error
    # exception if the input string is not valid UTF-8.
    #
    # Normally you don't need to invoke this method manually because the
    # parse method automatically sets up a scanner and obtains tokens as
    # it needs them. This method exists for testing and introspection
    # only.
    def tokenize string
      # This is just a placeholder.
      # See parser.c for the C source code to this method.
    end

    # Like the tokenize method feeds +string+ into the scanner to obtain
    # the corresponding tokens, but unlike the tokenize method it does not
    # return them because its sole purpose is to measure the speed of
    # the scanner.
    #
    # Just like the tokenize method raises a Wikitext::Parser::Error if
    # passed invalid UTF-8 input.
    def benchmarking_tokenize string
      # This is just a placeholder.
      # See parser.c for the C source code to this method.
    end

    # Parses and transforms the UTF-8 wikitext markup input string into
    # HTML. Raises a Wikitext::Parser::Error if passed invalid UTF-8.
    # You can customize some aspects of the transformation by setting
    # attributes on the parser instance before calling this method.
    #
    # See the attributes documentation for the Parser class.
    def parse string
      # This is just a placeholder.
      # See parser.c for the C source code to this method.
    end

    # Like the parse method transforms the UTF-8 input +string+ from
    # wikitext markup into HTML, but doesn't return a result. This
    # method is specifically designed for performance profiling so
    # you shouldn't need to call it in practice. It parses the supplied
    # string 100,000 times so as to provide enough sample data to make
    # profiling useful while minimizing the influence of extraneous
    # factors such as set-up code.
    def profiling_parse string
      # This is just a placeholder.
      # See parser.c for the C source code to this method.
    end

    # Exception raised when an error occurs during parsing.
    # As the parser is designed to gracefully cope with bad syntax, the
    # only reason you should see this exception is if you pass
    # invalidly-encoded UTF-8 to the parse method.
    class Error < Exception
    end

    # Token object representing a symbol found in the input stream during
    # scanning. When you invoke the tokenize method you receive an array
    # of Token instances.
    #
    # This class exists purely for testing and diagnostic purposes; it
    # is actually just a wrapper for the real token structure that is
    # used internally.  (In actual use the Wikitext extension doesn't
    # even use this class; it instead uses lightweight C structs under
    # the hood for maximum speed and memory efficiency.)
    #
    # = Attributes
    #
    # +start+::         the location in memory (a character pointer
    #                   into the input stream) where the token begins
    # +stop+::          the location in memory (a character pointer)
    #                   where the token ends
    # +line_start+::    the line number where the token starts;
    #                   numbering begins at line 1 (there is no line 0)
    # +line_stop+::     the line number where the token ends
    # +column_start+::  the column number where the token start;
    #                   numbering beings at column 1 (there is no column 0)
    # +column_stop+::   the column number where the token ends
    # +code_point+::    for tokens outside the range of printable ASCII
    #                   the UTF-32 code point corresponding to the token
    # +token_type+::    the type of the token, from the possible set of
    #                   token types returned by the types method
    # +string_value+::  the textal content of the token as a Ruby String
    class Token
      # Returns a hashof all token types by (numeric) value and
      # (human-readable) name.
      def self.types
        # This is just a placeholder.
        # See token.c for the C source code to this method.
      end
    end
  end
end
