

/******************************************************************************
*web���������ڵ�����
******************************************************************************/

#ifndef __WEBPAGE_H
#define __WEBPAGE_H
#define INDEX_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<meta http-equiv='Content-Type' content='text/html; charset=GB2312'/>"\
"<title>LXI����ϵͳ��ҳ</title>"\
"<style>"\
"::selection{background:red;color:#fff;}"\
"::-moz-selection{background:red;color:#fff;}"\
"::-webkit-selection{background:red;color:#fff;}"\
"body,div,h1,h2,h3{margin: 0; padding: 0; border: 0;}"\
"body{background: #474747;text-align:center;}"\
"#inset h1{"\
"text-align: center;"\
"margin: 0px auto;"\
"font-family:'����','����';"\
"font-size: 100px; text-transform: uppercase;"\
"color: #222;"\
"text-shadow: 0px 2px 3px #666;"\
"display:block;}"\
"h1 a:link{text-decoration:none; background:red;color:#fff}"\
"h1 a:hover{color: #fff; text-shadow: 0px 5px 8px #2a2a2a; text-decoration:none;}"\
"h1 a:visited{color:#fff;}"\
"#inset{margin-top:150px;margin-right:auto;margin-left:auto;}"\
".foot{margin-top:20px;font-size:10px;font-family:'arial';color:#222;}"\
"</style>"\
"</head>"\
"<body>"\
"<div id='inset'>"\
"<h1><a href='dream.html' target='_blank'>����</a></h1>"\
"<h1>  </h1>"\
"<h1><a href='config.html' target='_blank'>W5300��ʼ��</a></h1>"\
"<h1>  </h1>"\
"<h3 class='foot'>&copy;2016 ���޺�����</h3>"\
"</div>"\
"</body>"\
"</html>"\

#define DREAM_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<table width='1500' border='1'>"\
"<tr>"\
"<td colspan='2' style='background-color:#99bbbb;text-align:center;'>"\
"<h1>����ע��ģ��</h1>"\
"</td>"\
"</tr>"\
"<tr valign='top'>"\
"<td style='background-color:#ffff99;height:500px;width:100px;text-align:top;'>"\
"<b>Menu</b><br />"\
"HTML<br />"\
"CSS<br />"\
"JavaScript"\
"</td>"\
"<td style='background-color:#EEEEEE;height:200px;width:400px;text-align:top;'>"\
"<!--����������ȷ�ϰ�ť�ı�-->"\
"<form action='/example/html/form_action.asp' method='get'>"\
"<p>������ֵ: <input type='text' name='OHMValue' /></p>"\
"<p>����Ƶ��: <input type='text' name='FRQValve' /></p>"\
"</form>"\
"����ѡ��"\
"<form name='input' action='/html/html_form_action.asp' method='get'>"\
"Normal: "\
"<input type='radio' name='Sex' value='Normal' checked='checked'>"\
"<br />"\
"Vcc: "\
"<input type='radio' name='Sex' value='Vcc'>"\
"<br />"\
"Gnd: "\
"<input type='radio' name='Sex' value='Gnd'>"\
"<br />"\
"Sensor: "\
"<input type='radio' name='Sex' value='Sensor'>"\
"<br />"\
"<input type ='submit' value ='Send'>"\
"</form> "\
"<div style='margin-top:30px;'>"\
"</div>"\
"   <div style='background-color:#0066CC;color:#fff;margin-top:30px; '>"\
 "               <h3  >"\
 "                 <span >��ѹ���</span>"\
 "               </h3>"\
"</div>"\
"<div align='center'>"\
"<table border='1' cellpadding='10'>"\
"<tr>"\
 " <td width='200'>3.3V��ѹֵ</td>"\
 " <td width='200'><!--#t-->mv</td>"\
"</tr>   "\
"</table>"\
"<br><br>"\
"<table border='1' cellpadding='10'> "\
"<tr>"\
 " <td width='200'>5V��ѹֵ<</td>"\
 " <td width='200'><!--#w-->&deg;C</td>"\
"</tr>"\
"</table>"\
"<br><br>"\
" </div>"\
"</head>"\
"</html>"\


#define CONFIG_HTML  "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>W5300:��ҳ����</title>"\
"<meta http-equiv='Content-Type' content='text/html; charset=GB2312'/>"\
"<style type='text/css'>"\
"body {text-align:left; background-color:#c0deed;font-family:Verdana;}"\
"#main {margin-right:auto;margin-left:auto;margin-top:30px;}"\
"label{display:inline-block;width:150px;}"\
"#main h3{color:#66b3ff; text-decoration:underline;}"\
"</style>"\
"<script>"\
"function $(id) { return document.getElementById(id); };"\
"function settingsCallback(o) {"\
"if ($('txtVer')) $('txtVer').value = o.ver;"\
"if ($('txtMac')) $('txtMac').value = o.mac;"\
"if ($('txtIp')) $('txtIp').value = o.ip;"\
"if ($('txtSub')) $('txtSub').value = o.sub;"\
"if ($('txtGw')) $('txtGw').value = o.gw;"\
"};"\
"</script>"\
"</head>"\
"<body>"\
"<div id='main'>"\
"<div style='background:snow; display:block;padding:10px 20px;'>"\
"<h3>�����������</h3>"\
"<form id='frmSetting' method='POST' action='config.cgi'>"\
"<p><label for='txtIp'>�̼��汾��:</label><input type='text' id='txtVer' name='ver' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>MAC��ַ:</label><input type='text' id='txtMac' name='mac' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>IP��ַ:</label><input type='text' id='txtIp' name='ip' size='16' /></p>"\
"<p><label for='txtSub'>��������:</label><input type='text' id='txtSub' name='sub' size='16' /></p>"\
"<p><label for='txtGw'>Ĭ������:</label><input type='text' id='txtGw' name='gw' size='16' /></p>"\
"<p><input type='submit' value='���沢����' /></p>"\
"</form>"\
"</div>"\
"</div>"\
"<div style='margin:5px 5px;'>"\
"&copy; 2016 ���޺�����"\
"</div>"\
"<script type='text/javascript' src='w5500.js'></script>"\
"</body>"\
"</html>"\

#endif
