var    express = require('express');
var    module1 = require('./build/Release/module1');


var app = express();

console.log(module1.test());

app.get('/hello.action', function (req, res) {
  res.send('Hello World!');
});

app.use(express.static('../html'));

app.listen(8080, function () {
  console.log('Server started on port 8080!');
});
