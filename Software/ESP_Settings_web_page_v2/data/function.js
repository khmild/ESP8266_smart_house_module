
var xmlHttp=createXmlHttpObject();

 //---------------------------------------------------
function readTextFile(file, callback) {
	var rawFile = new XMLHttpRequest();
	rawFile.overrideMimeType("application/json");
	rawFile.open("GET", file, true);
	rawFile.onreadystatechange = function() {
		if (rawFile.readyState === 4 && rawFile.status == "200") {
			callback(rawFile.responseText);
		}
	}
	rawFile.send(null);
}


function save_wifi(submit){
	server = "/ssid?ssid="+val('ssid')+
	"&password="+encodeURIComponent(val('password'));
	
	send_request(submit,server);
	alert("Please restart for changes apply.");
}


function save_mqtt(submit){
	server = "/mqtt?mqttUser="+val('mqttUser')+
	"&mqttPassword="+encodeURIComponent(val('mqttPassword'))+
	"&ControllerIP="+val('ControllerIP')+
	"&ControllerPort="+val('ControllerPort')+
	"&ControllerSubscribe="+val('ControllerSubscribe')+
	"&ControllerPublish="+val('ControllerPublish');
	
	send_request(submit,server);
	alert("Please restart for changes apply.");
}


function set(){	
	
	readTextFile("/config.json", function(text){
		var data = JSON.parse(text);
		
		var element = document.getElementById("ssid");
		element.setAttribute('value', data.ssid);
		
		element = document.getElementById("password");
		element.setAttribute('value', data.password);
		
		element = document.getElementById("mqttUser");
		element.setAttribute('value', data.mqttUser);
		
		element = document.getElementById("mqttPassword");
		element.setAttribute('value', data.mqttPassword);
		
		element = document.getElementById("ControllerIP");
		element.setAttribute('value', data.mqttContrIP);
		
		element = document.getElementById("ControllerPort");
		element.setAttribute('value', data.mqttContrPort);
		
		element = document.getElementById("ControllerSubscribe");
		element.setAttribute('value', data.mqttContrSub);
		
		element = document.getElementById("ControllerPublish");
		element.setAttribute('value', data.mqttContrPub);
		
	});
}
 //---------------------------------------------------

function createXmlHttpObject(){
 if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
 }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}


function load(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/configs.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   loadBlock();
  }
 }
}


function loadBlock(data2) {
 data2 = JSON.parse(xmlHttp.responseText);
 data = document.getElementsByTagName('body')[0].innerHTML;
 var new_string;
for (var key in data2) {
 new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
 data = new_string;
}
 document.getElementsByTagName('body')[0].innerHTML = new_string;
 handleServerResponse();
}


function val(id){
 var v = document.getElementById(id).value;
 return v;
}


function send_request(submit,server){
 request = new XMLHttpRequest();
 request.open("GET", server, true);
 request.send();
 save_status(submit,request);
}


function save_status(submit,request){
 old_submit = submit.value;
 request.onreadystatechange = function() {
  if (request.readyState != 4) return;
  submit.value = request.responseText;
  setTimeout(function(){
   submit.value=old_submit;
   submit_disabled(false);
  }, 1000);
 }
 submit.value = 'Please wait...';
 submit_disabled(true);
}


function submit_disabled(request){
 var inputs = document.getElementsByTagName("input");
 for (var i = 0; i < inputs.length; i++) {
  if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
 }
}


function toggle(target) {
 var curVal = document.getElementById(target).className;
 document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}
