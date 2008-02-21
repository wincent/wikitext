require 'wikitext'

class String

  def to_wikitext
    @@shared_wikitext_parser ||= Wikitext::Parser.new(:space_to_underscore => true)
    @@shared_wikitext_parser.parse wikitext_preprocess
  end

  # Convenience shortcut
  alias :w :to_wikitext

private

  # for now do this in pure Ruby
  # if speed later becomes a concern can whip up a Ragel C extension to do it
  def wikitext_preprocess
    gsub /\b(bug|issue|request) #(\d+)/i, '[[issues/\2|\1 #\2]]'
  end

end
