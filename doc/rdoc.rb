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
  # For example, given an +internal_link_prefix+ of "/wiki/", the internal
  # link:
  #
  #     !!!wikitext
  #     [[Apple]]
  #
  # would be transformed into:
  #
  #     !!!html
  #     <a href="/wiki/Apple">Apple</a>
  #
  # == +external_link_class+ (String)
  #
  # The CSS class to be applied to external links (defaults to "external").
  # For example, given an +external_link_class+ of "external", the external
  # link:
  #
  #     !!!wikitext
  #     [http://www.google.com/ the best search engine]
  #
  # would be transformed into:
  #
  #     !!!html
  #     <a class="external" href="http://www.google.com/">the best search engine</a>
  #
  # == +external_link_rel+ (String)
  #
  # The +rel+ attribute to be applied to external links (defaults to +nil+,
  # meaning that no +rel+ attribute is applied). Setting a +rel+ attribute of
  # "nofollow" may be useful for search-engine optimization (see
  # http://en.wikipedia.org/wiki/Nofollow for more details).
  #
  # This attribute can be set during initialization:
  #
  #     parser = Wikitext::Parser.new :external_link_rel => 'nofollow'
  #
  # Or via setting an attribute on the parser:
  #
  #     parser = Wikitext::Parser.new
  #     parser.external_link_rel = 'nofollow'
  #
  # Or at parse time:
  #
  #     parser = Wikitext::Parser.new
  #     parser.parse input, :external_link_rel => 'nofollow'
  #
  # Setting +external_link_rel+ to +nil+ suppresses the emission of any
  # previously configured +rel+ attribute:
  #
  #     parser.parse input, :external_link_rel => nil
  #
  # == +mailto_class+ (String)
  #
  # The CSS class to be applied to external "mailto" links (defaults to
  # "mailto"). For example:
  #
  #     !!!wikitext
  #     [mailto:user@example.com user@example.com]
  #
  # or if autolinking of email addresses is turned on, just:
  #
  #     !!!wikitext
  #     user@example.com
  #
  # would be transformed into:
  #
  #     !!!html
  #     <a class="mailto" href="mailto:user@example.com">user@example.com</a>
  #
  # == +img_prefix+ (String)
  #
  # The prefix to be prepended to image tags (defaults to "/images/").
  # For example, given this image markup:
  #
  #     !!!wikitext
  #     {{foo.png}}
  #
  # The following +img+ tag would be produced:
  #
  #     !!!html
  #     <img src="/images/foo.png" alt="foo.png" />
  #
  # == +autolink+ (boolean)
  #
  # Whether to autolink URIs found in the plain scope.
  # When true:
  #
  #     !!!wikitext
  #     http://apple.com/
  #
  # will be transformed to:
  #
  #     !!!html
  #     <a href="http://apple.com/">http://apple.com/</a>
  #
  # and if an external_link_class is set (to "external", for example) then
  # the transformation will be:
  #
  #     !!!html
  #     <a class="external" href="http://apple.com/">http://apple.com/</a>
  #
  # When false, no transformation will be applied and the link will be
  # echoed literally:
  #
  #     !!!html
  #     http://apple.com/
  #
  # == +pre_code+ (boolean)
  #
  # When true, "pre" blocks are formatted using "code" elements. For example:
  #
  #     !!!wikitext
  #     <pre>foo</pre>
  #
  # Produces:
  #
  #     !!!html
  #     <pre><code>foo</code></pre>
  #
  # When +false+ (the default), it produces:
  #
  #     !!!html
  #     <pre>foo</pre>
  #
  # == +space_to_underscore+ (boolean)
  #
  # Whether spaces in link targets should be encoded normally or transformed
  # into underscores.
  #
  # When false, an internal link like:
  #
  #     !!!wikitext
  #     [[foo bar]]
  #
  # Would be converted into:
  #
  #     !!!html
  #     <a href="/wiki/foo%20bar">foo bar</a>
  #
  # But when true (the default), it would be converted into:
  #
  #     !!!html
  #     <a href="/wiki/foo_bar">foo bar</a>
  #
  # Converting spaces to underscores makes most URLs prettier, but it comes at
  # a cost: when this mode is true the articles "foo bar" and "foo_bar" can no
  # longer be disambiguated, and a link to "foo_bar" will actually resolve to
  # "foo bar"; it is therefore recommended that you explicitly disallow
  # underscores in titles at the application level so as to avoid this kind of
  # confusion.
  #
  # == +base_heading_level+ (integer)
  #
  # An integer between 0 and 6 denoting the current "heading level".
  # This can be used to inform the parser of the "context" in which
  # it is translating markup.
  #
  # For example, the parser might be translating blog post excerpts
  # on a page where there is an "h1" title element for the page itself
  # and an "h2" title element for each excerpt. In this context it is
  # useful to set +base_heading_level+ to 2, so that any "top level"
  # headings in the markup (that is "h1" elements) can be automatically
  # transformed into "h3" elements so that they appear to be
  # appropriately "nested" inside the containing page elements.
  #
  # In this way, markup authors can be freed from thinking about
  # which header size they should use and just always start from "h1"
  # for their most general content and work their way down.
  #
  # An additional benefit is that markup can be used in different
  # contexts at different levels of nesting and the headings will be
  # adjusted to suit automatically with no intervention from the
  # markup author.
  #
  # Finally, it's worth noting that in contexts where the user input
  # is not necessarily trusted, this setting can be used to prevent
  # users from inappropriately employing "h1" tags in deeply-nested
  # contexts where they would otherwise disturb the visual harmony of
  # the page.
  #
  # == +output_style+ (Symbol)
  #
  # Wikitext emits valid HTML5 fragments. By default, the output syntax is
  # HTML. Optionally, the output syntax can be changed to XML by setting the
  # +output_style+ to ":xml".
  #
  # This can be done during initialization:
  #
  #     parser = Wikitext::Parser.new :output_style => :xml
  #
  # Or via setting an attribute on the parser:
  #
  #     parser = Wikitext::Parser.new
  #     parser.output_style = :xml
  #
  # Or at parse time:
  #
  #     parser = Wikitext::Parser.new
  #     parser.parse input, :output_style => :xml
  #
  # In practice the only difference between the two output syntaxes is that
  # the XML syntax uses self closing +img+ tags:
  #
  #     !!!html
  #     <img src="foo.png" alt="Foo" />
  #
  # While the HTML syntax does not:
  #
  #     !!!html
  #     <img src="foo.png" alt="Foo">
  #
  # == +link_proc+ (lambda or Proc object)
  #
  # "Red links" can be implemented by providing a custom +link_proc+ block
  # at parse time. This can be used to check for existing or non-existent
  # link targets and apply custom CSS styling accordingly. For example,
  # consider:
  #
  #     link_proc = lambda { |target| target == 'bar' ? 'redlink' : nil }
  #     Wikitext::Parser.new.parse '[[foo]] [[bar]]', :link_proc => link_proc
  #
  # This would add the "redlink" CSS class to the "bar" link but not the
  # "foo" link. Please note that if your +link_proc+ involves database
  # queries then you should implement an appropriate caching strategy to
  # ensure that markup with many links does not overwhelm your database.
  #
  # A +link_proc+ may also be set during initialization:
  #
  #     parser = Wikitext::Parser.new :link_proc => link_proc
  #
  # Or via setting an attribute on the parser:
  #
  #     parser = Wikitext::Parser.new
  #     parser.link_proc = link_proc
  #
  # Many more examples of link procs can be found in the spec suite:
  #
  # * http://git.wincent.com/wikitext.git/blob/HEAD:/spec/internal_link_spec.rb
  class Parser

    # Sanitizes an internal link target for inclusion within the HTML
    # stream. Expects +string+ to be UTF-8-encoded.
    #
    # For example, a link target for the article titled:
    #
    #     !!!wikitext
    #     foo, "bar" & baz €
    #
    # would be sanitized as:
    #
    #     !!!html
    #     foo, &quot;bar&quot; &amp; baz &#x20ac;
    #
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
    #
    #     !!!wikitext
    #     foo, "bar" & baz €
    #
    # would be encoded as:
    #
    #     !!!html
    #     foo%2c%20%22bar%22%20%26%20baz%e2%82%ac
    #
    # The encoding is based on RFCs 2396 and 2718. The "unreserved" characters
    # a..z, a..Z, 0..9, "-", "_", "." and "~" are passed through unchanged and
    # all others are converted into percent escapes.
    #
    # When combined with sanitize_link_target this method can be used to emit
    # the following link for the example article:
    #
    #     !!!html
    #     <a href="foo%2c%20%22bar%22%20%26%20baz%e2%82%ac">foo, &quot;bar&quot; &amp; baz &#x20ac;</a>
    #
    # Note that when +space_to_underscore+ is +true+ spaces are treated specially,
    # and are converted to "_" rather than "%20". For the majority of links this
    # yields much prettier URLs at the cost of some reduction in the namespace of
    # possible titles (this is because when using +space_to_underscore+ you should
    # disallow underscores in article titles to avoid ambiguity between titles like
    # "foo bar" and "foo_bar").
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
    #
    #     parser = Wikitext::Parser.new
    #     parser.autolink = false
    #     parser.mailto_class = 'mail'
    #
    # And:
    #
    #     parser = Wikitext::Parser.new :autolink => false, :mailto_class => 'mail'
    #
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
    # attributes on the parser instance before calling this method
    # (see the attributes documentation for the Parser class),
    # or by passing in an (optional) options hash.
    #
    # Options that can be overridden at parse-time include:
    #
    # +indent+::              A non-negative number (to add an arbitrary
    #                         amount of indentation to all lines in the
    #                         output) or false (to disable indentation
    #                         entirely).
    # +base_heading_level+::  An integer between 0 and 6 denoting the
    #                         current "heading level" (documented above).
    # +output_style+::        A symbol, ":xml", to emit XML syntax (by
    #                         default HTML syntax is emitted)
    # +link_proc+::           A lambda that can be used to apply custom
    #                         CSS to links to produce "red links"
    #                         (documented above)
    def parse string, options = {}
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
