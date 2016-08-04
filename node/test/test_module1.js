var assert = require('assert');
var module1 = require('../build/Release/module1');

describe('module1', function() {
  describe('call', function() {
    it('should get message "hello world"', function(done) {
      module1.call((msg) => {
        assert.equal(msg, 'hello world', 'Get hello world message from call back.');
        done();
      });
    });
  });
  describe('test', function() {
    it('should get 3', function() {
      assert.equal(3, module1.test(1, 2), 'Get 3.');
    });
  });
  describe('newInstance', function() {
    it('Should get "What\'s up?"', function() {
      assert.equal('What\'s up?', module1.newInstance('What\'s up?').msg, 'Get "What\'s up?".');
    });
  });
  describe('plusOne', function() {
    it('should get 21 via plusOne.', function() {
      var obj = new module1.Hello(10);

      for(var i = 0; i < 10; ++i) {
        obj.plusOne();
      }
      
      assert.equal(21, obj.plusOne(), 'Get 21.');
    });
  });
  
  describe('getCPUTemperature', function() {
    it('should get the temperature.', function() {
      var temperature = module1.getCPUTemperature();
      console.log('temperature : %d', temperature);
    });
  });
  describe('getProcessMemoryUsage', function() {
    it('should get the memory usage.', function() {
      var usage = module1.getProcessMemoryUsage();
      console.log('usage : %f', usage);
    });
  });
  
});
