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

module RailsSpecs
  TRASH_PATH              = Pathname.new(__FILE__).dirname + '.trash'
  AREL_CLONE_PATH         = TRASH_PATH + 'arel.git'
  AREL_REPO               = 'git://github.com/rails/arel.git'
  RAILS_CLONE_PATH        = TRASH_PATH + 'rails.git'
  RAILS_REPO              = 'git://github.com/rails/rails.git'
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

  def clone repo, path
    if File.exist? path
      FileUtils.cd path do
        run 'git', 'fetch'
      end
    else
      run 'git', 'clone', repo, path
    end
  end

  def app_path version
    version = 'edge' if version.nil?
    version = "v#{version}" if version =~ /\A\d\./
    TRASH_PATH + "#{version}-app"
  end

  # if version is nil will create an "Edge" app
  def create_rails3_app rails_version, arel_version = nil
    app = app_path rails_version
    clone AREL_REPO, AREL_CLONE_PATH
    FileUtils.cd AREL_CLONE_PATH do
      if arel_version
        run 'git', 'reset', '--hard', "v#{arel_version}"
      else # "Edge"
        run 'git', 'reset', '--hard', 'origin/master'
      end
      run 'git', 'clean', '-f'
    end

    clone RAILS_REPO, RAILS_CLONE_PATH
    FileUtils.cd RAILS_CLONE_PATH do
      if rails_version
        run 'git', 'reset', '--hard', "v#{rails_version}"
      else # "Edge"
        run 'git', 'reset', '--hard', 'origin/master'
      end
      run 'git', 'clean', '-f'

      begin
        clean_bundler_environment
        run 'env', "AREL=#{AREL_CLONE_PATH}",
            'bundle', 'install', '--path', '../bundle', '--without', 'db'
        FileUtils.rm_r(app) if File.exist?(app)
        run 'env', "AREL=#{AREL_CLONE_PATH}",
            'bundle', 'exec', 'bin/rails', 'new', app, '--skip-activerecord', '--dev'
      ensure
        restore_bundler_environment
      end
    end

    create_gemfile app
    bundlerize app
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

  def add_text_to_routes text, infile
    insert text, 'Application.routes.draw', infile
  end

  def create_gemfile app
    File.open(app + 'Gemfile', 'w') do |f|
      f.write <<-GEMFILE
        source :rubygems
        gem 'arel', :path => "#{AREL_CLONE_PATH.realpath}"
        gem 'rake'
        gem 'rails', :path => "#{RAILS_CLONE_PATH.realpath}"
        gem 'sqlite3'
        gem 'wikitext', :path => "#{WIKITEXT_GEM_PATH.realpath}"
      GEMFILE
    end
  end

  def bundlerize app
    clean_bundler_environment
    Dir.chdir app do
      run 'bundle', 'install', '--path', '../bundle', '--binstubs'
    end
  ensure
    restore_bundler_environment
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

  def setup_rails_app rails_version = nil, arel_version = nil
    create_rails3_app rails_version, arel_version
    path = app_path rails_version
    update_routes path
    create_controller path
    create_template path
    create_test path
  end

  def clean_bundler_environment
    @bundler_env = ENV.select { |key, value| key =~ /\A(BUNDLE|GEM)_/ }
    @bundler_env.each { |pair| ENV.delete(pair.first) }
  end

  def restore_bundler_environment
    @bundler_env.each { |pair| ENV[pair[0]] = pair[1] }
  end

  def run_integration_test app
    clean_bundler_environment
    FileUtils.cd app do
      return run('bin/rake', 'test:integration').stdout
    end
  ensure
    restore_bundler_environment
  end
end # module RailsSpecs

# different versions of Rails require different versions of Arel
{ '3.1.0' => '2.1.1' }.each do |rails_version, arel_version|
  describe "Template handler in Rails #{rails_version}" do
    include RailsSpecs

    before :all do
      setup_rails_app rails_version, arel_version
      @path = app_path rails_version
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
