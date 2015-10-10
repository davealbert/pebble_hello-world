var initialized = false;
var options = {};

Pebble.addEventListener("ready", function() {
  console.log("ready called!");
  initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
  console.log("showing configuration");
  Pebble.openURL('https://one15digital.com/pebble/'+ encodeURIComponent(JSON.stringify(options)));
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("configuration closed: ", e);
  // webview closed
  //Using primitive JSON validity and non-empty check
  if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
    options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
  } else {
    console.log("Cancelled");
  }

  var dict = { "name": "dwa" };

  // Send settings to Pebble watchapp
  Pebble.sendAppMessage(dict, function(){
     console.log('Sent config data to Pebble');
  }, function() {
     console.log('Failed to send config data!');
  });
});
