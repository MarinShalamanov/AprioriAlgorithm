/*
var globalTunnel = require('global-tunnel');

globalTunnel.initialize({
  host: 'proxy',
  port: 8080
});
*/

var LastfmAPI = require('lastfmapi');
var lfm = new LastfmAPI({
    'api_key' : '72b3d4d46ac309961203da919410d8e4',
    'secret' : '32ff6336971cc288390f0eb14f00b52a'
});


var processed = new Set();

var stack = [];

var fs = require('fs');

function proccessUser(user) {
	
	if(processed.has(user)) return;
	
	processed.add(user);
	
	lfm.user.getLovedTracks({
		'user' : user,
		'limit' : '1000'
	}, function (err, track) {
		if (err) { console.log(err); }
		else {
			var arr = track.track;
			
			console.log(user + "    Num tracks: ", arr.length);
			for(var i = 0; i < arr.length; i++) {
				fs.appendFile("usersData/" + user, arr[i].name + "\r\n", null);
				//console.log(arr[i].name);
			}
		}
		//console.log(track.track);
	});
	
	
	lfm.user.getFriends({
		'user' : user,
		'limit' : '500'
	}, function (err, track) {
		if (err) { console.log(err); }
		else {
			
			var arr = track.user;
			console.log(user + "    Num users: ", arr.length);
			for(var i = 0; i < arr.length; i++) {
				stack.push(arr[i].name);
				//console.log(arr[i].name);
			} 
		}
	});
	
	
}


i = 100;

function start () {

	if(i-- > 0) {
		var currUser = stack.pop();
		proccessUser(currUser);
		
		setTimeout(start, 500);
		
		console.log(stack.length + " users in the stack");
	}
}


stack.push("tkumars7");
start()

return;

lfm.track.getInfo({
    'artist' : 'Sia',
    'track' : 'Chandelier'
}, function (err, track) {
    if (err) { console.log(err); }
    console.log(track.duration);
});


lfm.user.getFriends({
    'user' : 'tkumars7',
	'limit' : '500'
}, function (err, track) {
    if (err) { console.log(err); }
	else {
		
		var arr = track.user;
		console.log("Num users: ", arr.length);
		for(var i = 0; i < arr.length; i++) {
			console.log(arr[i].name);
		} 
	}
    //console.log(track.user);
});

lfm.user.getLovedTracks({
    'user' : 'tkumars7',
	'limit' : '1000'
}, function (err, track) {
    if (err) { console.log(err); }
	else {
		var arr = track.track;
		console.log("Num tracks: ", arr.length);
		for(var i = 0; i < arr.length; i++) {
			console.log(arr[i].name);
		}
	}
    //console.log(track.track);
});
