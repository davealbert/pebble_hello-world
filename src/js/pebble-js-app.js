var initialized = false;
var options = {};

Pebble.addEventListener("ready", function() {
  console.log("ready called!");
  initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
  var url = 'https://one15digital.com/pebble/?'+ encodeURIComponent(JSON.stringify(options))
  console.log("showing configuration", url);
  Pebble.openURL(url);
});

Pebble.addEventListener("appmessage", function(e) {
   console.log('appmessage: ', e);
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("configuration closed: ", e.response);
  // webview closed
  //Using primitive JSON validity and non-empty check
  var name = "Still waiting...";
  var dict = {};

  if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
    options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
    if (options.name && options.name !== "") {
       var dict = {"KEY_LOCATION": options.name};
    }
  } else {
    console.log("Cancelled");
  }

  // Send settings to Pebble watchapp
  Pebble.sendAppMessage(dict);
});
