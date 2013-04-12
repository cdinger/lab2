# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "serialport"
  s.version = "1.1.0"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Guillaume Pierronnet", "Alan Stern", "Daniel E. Shipton", "Tobin Richard", "Hector Parra", "Ryan C. Payne"]
  s.date = "2012-05-26"
  s.description = "Ruby/SerialPort is a Ruby library that provides a class for using RS-232 serial ports."
  s.email = "hector@hectorparra.com"
  s.extensions = ["ext/native/extconf.rb"]
  s.extra_rdoc_files = ["LICENSE", "README"]
  s.files = ["LICENSE", "README", "ext/native/extconf.rb"]
  s.homepage = "http://github.com/hparra/ruby-serialport/"
  s.require_paths = ["lib"]
  s.rubygems_version = "1.8.23"
  s.summary = "Library for using RS-232 serial ports."

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end