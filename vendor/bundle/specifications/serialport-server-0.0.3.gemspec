# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "serialport-server"
  s.version = "0.0.3"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Sho Hashimoto"]
  s.date = "2012-05-23"
  s.description = "SerialPort Server makes your Device (Arduino, mbed...) WebServer. You can access SerialPort via HTTP, WebSocket and TCP Socket."
  s.email = ["hashimoto@shokai.org"]
  s.executables = ["serialport-server"]
  s.extra_rdoc_files = ["History.txt", "Manifest.txt", "README.rdoc"]
  s.files = ["bin/serialport-server", "History.txt", "Manifest.txt", "README.rdoc"]
  s.homepage = "http://shokai.github.com/serialport-server"
  s.post_install_message = "!! For more information on serialport-server, see http://shokai.github.com/serialport-server"
  s.rdoc_options = ["--main", "README.rdoc"]
  s.require_paths = ["lib"]
  s.rubyforge_project = "serialport-server"
  s.rubygems_version = "1.8.23"
  s.summary = "SerialPort Server makes your Device (Arduino, mbed...) WebServer"

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<serialport>, [">= 1.0.4"])
      s.add_runtime_dependency(%q<eventmachine>, [">= 0"])
      s.add_runtime_dependency(%q<eventmachine_httpserver>, [">= 0"])
      s.add_runtime_dependency(%q<em-websocket>, [">= 0"])
      s.add_runtime_dependency(%q<args_parser>, [">= 0"])
      s.add_runtime_dependency(%q<json>, [">= 0"])
      s.add_development_dependency(%q<rdoc>, ["~> 3.10"])
      s.add_development_dependency(%q<newgem>, [">= 1.5.3"])
      s.add_development_dependency(%q<hoe>, ["~> 2.15"])
    else
      s.add_dependency(%q<serialport>, [">= 1.0.4"])
      s.add_dependency(%q<eventmachine>, [">= 0"])
      s.add_dependency(%q<eventmachine_httpserver>, [">= 0"])
      s.add_dependency(%q<em-websocket>, [">= 0"])
      s.add_dependency(%q<args_parser>, [">= 0"])
      s.add_dependency(%q<json>, [">= 0"])
      s.add_dependency(%q<rdoc>, ["~> 3.10"])
      s.add_dependency(%q<newgem>, [">= 1.5.3"])
      s.add_dependency(%q<hoe>, ["~> 2.15"])
    end
  else
    s.add_dependency(%q<serialport>, [">= 1.0.4"])
    s.add_dependency(%q<eventmachine>, [">= 0"])
    s.add_dependency(%q<eventmachine_httpserver>, [">= 0"])
    s.add_dependency(%q<em-websocket>, [">= 0"])
    s.add_dependency(%q<args_parser>, [">= 0"])
    s.add_dependency(%q<json>, [">= 0"])
    s.add_dependency(%q<rdoc>, ["~> 3.10"])
    s.add_dependency(%q<newgem>, [">= 1.5.3"])
    s.add_dependency(%q<hoe>, ["~> 2.15"])
  end
end
