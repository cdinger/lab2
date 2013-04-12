# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "args_parser"
  s.version = "0.1.4"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Sho Hashimoto"]
  s.date = "2013-01-26"
  s.description = "Parse ARGV from command line with DSL."
  s.email = ["hashimoto@shokai.org"]
  s.homepage = "http://shokai.github.com/args_parser"
  s.require_paths = ["lib"]
  s.rubygems_version = "1.8.23"
  s.summary = "Parse ARGV from command line with DSL."

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
