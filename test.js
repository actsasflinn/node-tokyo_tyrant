// Funny Sort of test...
// node is evented, tyrant is blocking?

var sys=require("sys");
var posix=require('posix');
var tokyo_tyrant=require("./tokyo_tyrant");

var n = 15000;
var t = (new tokyo_tyrant.TokyoTyrant).init("127.0.0.1", 1978);

t.vanish();

var start = new Date();
for(i=0;i<n;i++){
  t.put("foo"+i, "bar"+i);
}
var elapsed = new Date() - start;
sys.puts("Put " + n + " records in " + elapsed + "ms");

var start = new Date();
for(i=0;i<n;i++){
  var res = t.get("foo"+i);
}
var elapsed = new Date() - start;
sys.puts("Get " + n + " records in " + elapsed + "ms");
