var express      = require('express'),
    path         = require('path'),
    bodyParser   = require('body-parser');

var index = require('./routes/index');

var app = express();
app.use(express.static('public'))
app.use('/', express.static(path.join(__dirname, 'index.html')))

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));

app.use('/',index);

app.listen(8080);