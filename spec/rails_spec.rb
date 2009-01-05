#!/usr/bin/env ruby
# Copyright 2009 Wincent Colaiuta
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

require File.join(File.dirname(__FILE__), 'spec_helper.rb')
require 'wikitext/version'
require 'fileutils'
require 'pathname'

# TODO: silence stdout for these commands; want the specs to run silently unless errors occur
module RailsSpecs
  TRASH_PATH    = Pathname.new(__FILE__).dirname + 'trash'
  CLONE_PATH    = TRASH_PATH + 'rails.git'
  RAILS_PATH    = CLONE_PATH + 'railties' + 'bin' + 'rails'
  EDGE_APP_PATH = TRASH_PATH + 'edge-app'

  def clone
    if File.exist? CLONE_PATH
      system "cd #{CLONE_PATH} && git fetch"
    else
      system "git clone git://github.com/rails/rails.git #{CLONE_PATH}"
    end
  end

  def app_path version
    TRASH_PATH + "#{version}-app"
  end

  def create_base_app_and_symlinks app, &block
    FileUtils.rm_r(app) if File.exist?(app)
    yield
    system "ruby #{RAILS_PATH} #{app}"
    vendor = app + 'vendor'
    gems = vendor + 'gems'
    system "cd #{vendor} && ln -s ../../rails.git rails"
    FileUtils.mkdir gems
    system "cd #{gems} && ln -s ../../../../.. wikitext-#{Wikitext::VERSION}" 
  end

  def create_release_app version
    create_base_app_and_symlinks app_path(version) do
      system "cd #{CLONE_PATH} && git checkout v#{version} && git clean -f"
    end
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

  def add_text_to_initializer text, infile
    insert text, 'Rails::Initializer.run do', infile 
  end

  def create_edge_app
    create_base_app_and_symlinks EDGE_APP_PATH do
      system "cd #{CLONE_PATH} && git checkout master && git merge origin/master && git clean -f"
    end
  end

  def update_environment app
    environment =  app + 'config' + 'environment.rb'
    add_text_to_initializer "  config.gem 'wikitext', :version => '#{Wikitext::VERSION}'", environment
    system "cd #{app} && rake gems:refresh_specs"
  end

  def setup_release_app version
    clone
    create_release_app version
    update_environment app_path(version)
  end

  def setup_edge_app
    clone
    create_edge_app
    update_environment EDGE_APP_PATH
  end

  # TODO: instead of actually launching server, leverage Rails integration tests
  def launch_release_app version
    system "cd #{app_path(version)} && ./script/server -d"
  end

  def launch_edge_app
    system "cd #{EDGE_APP_PATH} && ./script/server -d"
  end

  def stop_release_app version
    system "kill `cat #{app_path(version)}/tmp/pids/mongrel.pid`"
  end

  def stop_edge_app
    system "kill `cat #{EDGE_APP_PATH}/tmp/pids/mongrel.pid`"
  end
end # module RailsSpecs

describe 'Template handler in Rails 2.2.2' do
  include RailsSpecs
  version = '2.2.2'

  before(:all) do
    setup_release_app version
    launch_release_app version
    # wait for it to finish launching
  end

  after(:all) do
    stop_release_app version
  end

  it 'should process the template using the wikitext module' do
    pending
    # either use system "curl localhost:3000" here or some kind of module, Net::HTTP
  end
end

# TODO:
#
# create empty controller:
#   class FooController < ApplicationController
#   end
#
# and view:
#  apps/views/foo/index.html.wikitext
#
# with content:
#  * hello
#
# which should render as:
#   <ul>
#     <li>hello</li>
#   </ul>
