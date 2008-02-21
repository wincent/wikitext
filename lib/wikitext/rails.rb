require 'wikitext/string'

module Wikitext
  class TemplateHandler
    def initialize view
    end

    def render text, locals = {}
      text.w
    end
  end
end

ActionView::Base.register_template_handler :wikitext, Wikitext::TemplateHandler
