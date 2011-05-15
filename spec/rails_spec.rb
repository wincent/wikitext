# Copyright 2009-2011 Wincent Colaiuta. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.

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

require 'spec_helper'
require 'wikitext/version'
require 'fileutils'
require 'pathname'
require 'wopen3'
require 'ostruct'

module RailsSpecs
  TRASH_PATH              = Pathname.new(__FILE__).dirname + 'trash'
  CLONE_PATH              = TRASH_PATH + 'rails.git'
  RAILS_BIN_PATH          = CLONE_PATH + 'bin' + 'rails'
  WIKITEXT_GEM_PATH       = TRASH_PATH + '..' + '..'
  SUCCESSFUL_TEST_RESULT  = /1 tests, 3 assertions, 0 failures, 0 errors/

  def run cmd, *args
    result = Wopen3.system(*([cmd] + args))
    if result.status != 0
      command_string = ([cmd] + args).join(' ')
      puts "\n*** COMMAND #{command_string} EXITED WITH NON-ZERO EXIT STATUS (#{result.status})"
      puts "*** STDOUT FOR COMMAND #{command_string}:", result.stdout
      puts "*** STDERR FOR COMMAND #{command_string}:", result.stderr
      raise "non-zero exit status (#{result.status}) for '#{command_string}'"
    end
    result
  end

  def clone
    if File.exist? CLONE_PATH
      FileUtils.cd CLONE_PATH do
        run 'git', 'fetch'
      end
    else
      run 'git', 'clone', 'git://github.com/rails/rails.git', CLONE_PATH
    end
  end

  def app_path version
    version = 'edge' if version.nil?
    version = "v#{version}" if version =~ /\A3\./
    TRASH_PATH + "#{version}-app"
  end

  # if version is nil will create an "Edge" app
  def create_rails3_app version
    app = app_path version
    clone
    FileUtils.rm_r(app) if File.exist?(app)
    FileUtils.cd CLONE_PATH do
      if version
        run 'git', 'reset', '--hard', "v#{version}"
      else # "Edge"
        run 'git', 'reset', '--hard', 'origin/master'
      end
      run 'git', 'clean', '-f'
    end
    run 'ruby', RAILS_BIN_PATH, 'new', app, '--skip-activerecord', '--dev'
    create_gemfile app
  end

  def insert text, after, infile
    output = []
    found   = false
    File.read(infile).split("\n").each do |line|
      output << line
      if found == false && line =~ /#{Regexp.escape(after)}/
        found = true
        output << text
      end
    end
    File.open(infile, 'wb') { |f| f.write(output.join("\n")) }
    raise "text '#{after}' not found" unless found
  end

  # Rails 2 only
  def add_text_to_initializer text, infile
    insert text, 'Rails::Initializer.run do', infile
  end

  # Rails 3 only
  def add_text_to_routes text, infile
    insert text, 'Application.routes.draw', infile
  end

  def create_gemfile app
    File.open(app + 'Gemfile', 'w') do |f|
      f.write <<-GEMFILE
        gem 'rails', :path => "#{CLONE_PATH.realpath}"
        gem 'wikitext', :path => "#{WIKITEXT_GEM_PATH.realpath}"
      GEMFILE
    end
  end

  def create_controller app
    File.open(app + 'app' + 'controllers' + 'wiki_controller.rb', 'w') do |f|
      f.write 'class WikiController < ApplicationController; end'
    end
  end

  def create_template app
    template_dir = app + 'app' + 'views' + 'wiki'
    FileUtils.mkdir template_dir
    File.open(template_dir + 'index.html.wikitext', 'w') do |f|
      f.write '* hello, world!'
    end
  end

  def create_test app
    # integration tests won't run without a schema.rb
    FileUtils.touch app + 'db' + 'schema.rb'

    File.open(app + 'test' + 'integration' + 'wiki_test.rb', 'w') do |f|
      f.write <<'TEST'
require File.join(File.dirname(__FILE__), '..', 'test_helper')

class WikiTest < ActionController::IntegrationTest
  def test_wiki_index
    get "/wiki"
    assert_response :success
    assert_template "wiki/index"
    assert_select 'ul>li', 'hello, world!'
  end
end
TEST
      end
  end

  def update_routes app
    routes = app + 'config' + 'routes.rb'
    add_text_to_routes 'match "/wiki" => "wiki#index"', routes
  end

  def setup_rails_app version = nil
    create_rails3_app version
    path = app_path version
    update_routes path
    create_controller path
    create_template path
    create_test path
  end

  def run_integration_test app
    FileUtils.cd app do
      return run('rake', 'test:integration').stdout
    end
  end
end # module RailsSpecs

%w{3.0.1
   3.0.2
   3.0.3
   3.0.4
   3.0.5
   3.0.6
   3.0.7}.each do |version|
  describe "Template handler in Rails #{version}" do
    include RailsSpecs

    before :all do
      setup_rails_app version
      @path = app_path version
    end

    it 'should process the template using the wikitext module' do
      run_integration_test(@path).should =~ RailsSpecs::SUCCESSFUL_TEST_RESULT
    end
  end
end

describe 'Template handler in Edge Rails' do
  include RailsSpecs

  before :all do
    setup_rails_app
    @path = app_path nil
  end

  it 'should process the template using the wikitext module' do
    run_integration_test(@path).should =~ RailsSpecs::SUCCESSFUL_TEST_RESULT
  end
end
