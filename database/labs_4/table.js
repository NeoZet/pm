var http = require('http')

http.createServer(function(request, response) {
    response.writeHead(200, {'Content-Type':'text/html'});
    response.write('<!DOCTYPE html>\n' +
		   '<html>\n'+
		   '<head>\n' +
		   '<meta charset=\'utf-8\'>\n' +
		   '</head>\n'+
		   '<body>\n');
    response.write('<table border="1px" cellspacing="0px" width="30%" height="10%">\n'+
		   '<tr>\n'+
		   '<td style="color: black">\nФамилия Имя</td>\n'+
		   '<td id="str" align="center">\nВозрст</td>\n'+
		   '<td id="numb1" align="center">\nID</td>\n'+
		   '</tr>\n'+
		   '<tr>\n'+
		   '<td style="color: black">\n"Erofei Pavlov"</td>\n'+
		   '<td id="numb1" align="center">\n3</td>\n'+
		   '<td id="numb1" align="center">\n1722</td>\n'+
		   '</tr>\n'+
		   '<tr>\n'+
		   '<td style="color: black">\n"Erofei Pavlov"</td>\n'+
		   '<td id="numb1" align="center">\n18</td>\n'+
		   '<td id="numb1" align="center">\n1237</td>\n'+
		   '</tr>\n'+
		   '<tr>\n'+
		   '<td style="color: black">\n"Oleg Efimov"</td>\n'+
		   '<td id="numb1" align="center">\n33</td>\n'+
		   '<td id="numb1" align="center">\n1312</td>\n'+
		   '</tr>\n'+
		   '<tr>\n'+
		   '<td style="color: black">\n"Erofei Ivanov"</td>\n'+
		   '<td id="numb1" align="center">\n32</td>\n'+
		   '<td id="numb1" align="center">\n156</td>\n'+
		   '</tr>\n'+
		   '<tr>\n'+
		   '<td style="color: black">\n"Ivan Emin"</td>\n'+
		   '<td id="numb1" align="center">\n20</td>\n'+
		   '<td id="numb1" align="center">\n1345</td>\n'+
		   '</tr>\n'+
		   '<tr>\n'+
		   '<td style="color: black">\n"Andrey Novov"</td>\n'+
		   '<td id="numb1" align="center">\n22</td>\n'+
		   '<td id="numb1" align="center">\n179</td>\n'+
		   '</tr>\n'+
		   '<tr>\n'+
		   '<td style="color: black">\n"Genry Eva"</td>\n'+
		   '<td id="numb1" align="center">\n11</td>\n'+
		   '<td id="numb1" align="center">\n1997</td>\n'+
		   '</tr>\n'+
		   '<tr>\n'+
		   '<td style="color: black">\n"Jerry Smith"</td>\n'+
		   '<td id="numb1" align="center">\n23</td>\n'+
		   '<td id="numb1" align="center">\n1007</td>\n'+
		   '</tr>\n'+	   
		   '<tr>\n'+
		   '<td style="color: black">\n"Any Jhones"</td>\n'+
		   '<td id="numb1" align="center">\n44</td>\n'+
		   '<td id="numb1" align="center">\n1117</td>\n'+
		   '</tr>\n'+	   
		   '<tr>\n'+
		   '<td style="color: black">\n"Saimon Said"</td>\n'+
		   '<td id="numb1" align="center">\n25</td>\n'+
		   '<td id="numb1" align="center">\n1447</td>\n'+
		   '</tr>\n'+	   
		   '<tr>\n'+
		   '<td style="color: black">\n"Robert Bobert"</td>\n'+
		   '<td id="numb1" align="center">\n17</td>\n'+
		   '<td id="numb1" align="center">\n1437</td>\n'+
		   '</tr>\n'+	   
		   '</table>\n'+
		   ' </body>\n'+
		   '</html>\n');
}).listen(3000);

