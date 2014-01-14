# Copyright 2008-2014 Wincent Colaiuta. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

class String
  # This is an example preprocessing method that will be called by the
  # String#to_wikitext method prior to feeding the markup into the parser.
  # In this case we take input like this:
  #
  #   For more info see issue #125, which was filed last week.
  #
  # And transform it into:
  #
  #   For more info see [/issues/125 issue #125], which was filed last week.
  #
  # Which in turn is translated into the following HTML by the parser:
  #
  #   <p>For more info see <a href="/issues/125">issue #125</a>, which was
  #   filed last week.</p>
  #
  # Obviously this file is an example designed to work with a specific
  # application and is optional, so will only come into play if explicitly
  # required. To provide different behavior, simply define a new definition of
  # String#wikitext_preprocess.
  #
  # For now this is done in pure Ruby; if speed became a concern we could
  # provide a fast Ragel C extension to do it instead.
  def wikitext_preprocess
    gsub /\b(bug|issue|request|ticket) #(\d+)/i, '[/issues/\2 \1 #\2]'
  end
end
