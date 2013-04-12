require File.expand_path 'test_helper', File.dirname(__FILE__)

class TestArgsParserValidator < MiniTest::Unit::TestCase
  def setup
    @argv = ['--url', 'hptt://shokai.org']
    @@err = nil
    @@name = nil
    @@value = nil
    @parser = ArgsParser.parse @argv do
      arg :url, 'URL'

      validate :url, 'invalid URL' do |v|
        v =~ Regexp.new("^https?://.+$")
      end
      
      on_validate_error do |err, name, value|
        @@err = err
        @@name = name
        @@value = value
      end
    end
  end

  def test_validate_error
    assert @@name == :url
    assert @@value == 'hptt://shokai.org'
    assert @@err.class == ArgsParser::ValidationError
  end
end
