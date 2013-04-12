# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "eventmachine_httpserver"
  s.version = "0.2.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Francis Cianfrocca"]
  s.cert_chain = nil
  s.date = "2007-03-16"
  s.description = ""
  s.email = "garbagecat10@gmail.com"
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = ["docs/COPYING", "docs/README", "docs/RELEASE_NOTES"]
  s.files = ["docs/COPYING", "docs/README", "docs/RELEASE_NOTES", "ext/extconf.rb"]
  s.homepage = "https://github.com/eventmachine/evma_httpserver"
  s.rdoc_options = ["--title", "EventMachine_HttpServer", "--main", "docs/README", "--line-numbers"]
  s.require_paths = ["lib"]
  s.required_ruby_version = Gem::Requirement.new("> 0.0.0")
  s.rubygems_version = "1.8.23"
  s.summary = "EventMachine HTTP Server"

  if s.respond_to? :specification_version then
    s.specification_version = 1

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
