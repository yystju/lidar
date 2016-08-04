var      express = require('express');
var  compression = require('compression');

var app = express();

app.use(compression({filter: function (req, res) {
  if (req.headers['x-no-compression']) {
    return false;
  }
  
  return compression.filter(req, res);
}}));

app.get('/hello', function (req, res) {
  res.type('json');
  res.status(200).json({
    p1: 'Hello',
    p2: new Date(),
    p3: 5,
    p4: ['heihei']
  });
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
