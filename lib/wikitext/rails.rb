# Copyright 2008 Wincent Colaiuta
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

require 'wikitext/string'

module Wikitext
  class TemplateHandler
    def initialize view; end

    def render text, locals = {}
      text.w
    end
  end
end

if ActionView::Template.respond_to? :register_template_handler  # Rails 2.1.0_RC1 and above
  ActionView::Template.register_template_handler :wikitext, Wikitext::TemplateHandler
elsif ActionView::Base.respond_to? :register_template_handler   # Rails 2.0.2
  ActionView::Base.register_template_handler :wikitext, Wikitext::TemplateHandler
else
  raise "Incompatible Rails API version (can't find register_template_handler method)"
end
