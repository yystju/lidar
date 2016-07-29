var      express = require('express');
var  compression = require('compression');
var      module1 = require('./build/Release/module1');


var app = express();

app.use(compression({filter: function (req, res) {
  if (req.headers['x-no-compression']) {
    return false;
  }
  
  return compression.filter(req, res);
}}));

module1.call((msg) => {
  //console.log(msg);
  //console.log(module1.test(1, 2));
  //console.log(module1.newInstance('What\'s up?').msg);
  
  var obj = new module1.Hello(10);
  
  for(var i = 0; i < 10; ++i) {
	console.log(obj.plusOne());
  }
});

app.get('/hello.action', function (req, res) {
  res.send('Hello World!');
});

app.get('/events', function (req, res) {
  res.setHeader('Content-Type', 'text/event-stream');
  res.setHeader('Cache-Control', 'no-cache');

  var timer = setInterval(function () {
    res.write('data: ping\n\n');

    res.flush();
  }, 2000);

  res.on('close', function () {
    clearInterval(timer);
  });
});

app.use(express.static('../html'));

app.listen(8080, function () {
  console.log('Server started on port 8080!');
});
