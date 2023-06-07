function writeFile(str)
{
	var fso, ctf;
	try
	{
		if(window.ActiveXObject) {
			fso = new ActiveXObject("Scripting.FileSystemObject");
			ctf = fso.CreateTextFile("c:\\test.txt", true);
			ctf.Write (str);
			ctf.Close();
		}
	}
	catch (e) {
	}
}


function selectLine(id)
{
	var objTR = getElement(id);

	if (objTR != null)
	{
		var temp = objTR.id.split('_')[1];
		if (temp == 'null')
		{
		    setControl(-1);	
			setLineHighLight(objTR);
			setDisable('btnApply',0);	
			setDisable('btnCancel',0);		
		}
        else if (temp == 'no')
        {   
            setControl(-2);
            setDisable('btnApply',0);	
			setDisable('btnCancel',0);
        }
		else
		{
			var index = parseInt(temp);
			setControl(index);
            setLineHighLight(objTR);
			setDisable('btnApply',1);	
			setDisable('btnCancel',1);				
		}	
	}	 
}

function clickAdd(tabTitle)
{
	var tab = document.getElementById(tabTitle).getElementsByTagName('table');

	var row,col;
	var rowLen = tab[0].rows.length;
	var firstRow = tab[0].rows[0];
	var lastRow = tab[0].rows[rowLen - 1];
	
	if (lastRow.id == 'record_null')
	{
		selectLine("record_null");
		return;
	}
    else if (lastRow.id == 'record_no')
    {
        tab[0].deleteRow(rowLen-1);
        rowLen = tab[0].rows.length;
    }
	
	row = tab[0].insertRow(rowLen);	

	var appName = navigator.appName;
	if(appName == 'Microsoft Internet Explorer')
	{
		g_browserVersion = 1; /* IE */
		row.className = 'trTabContent';
		row.id = 'record_null';
		row.attachEvent("onclick", function(){selectLine("record_null");});
	}
	else
	{
		g_browserVersion = 2; /* firefox */
		row.setAttribute('class','trTabContent');
		row.setAttribute('id','record_null');
		row.setAttribute('onclick','selectLine(this.id);');
		row.setAttribute('style','text-align:center;') ;
	}
	
	for (var i = 0; i < firstRow.cells.length; i++)
	{
        col = row.insertCell(i);
	 	col.innerHTML = '--';
	} 
	selectLine("record_null");
}

function addNullInst(tabTitle)
{
    var tab = document.getElementById(tabTitle).getElementsByTagName('table');
	var row,col;
	var rowLen = tab[0].rows.length;
	var firstRow = tab[0].rows[0];
	var lastRow = tab[0].rows[rowLen - 1];
    
    tab[0].deleteRow(rowLen-1);
    rowLen = tab[0].rows.length;
    row = tab[0].insertRow(rowLen);

	var appName = navigator.appName;
	if (appName == 'Microsoft Internet Explorer')
	{
		g_browserVersion = 1; /* IE */
		row.className = 'trTabContent';
		row.id = 'record_no';
		row.attachEvent("onclick", function(){selectLine("record_no");});
	}
	else
	{
		g_browserVersion = 2; /* firefox */
		row.setAttribute('class','trTabContent');
		row.setAttribute('id','record_no');
		row.setAttribute('onclick','selectLine(this.id);');
		row.setAttribute('style','text-align:center;') ;
	}
	
	for (var i = 0; i < firstRow.cells.length; i++)
	{
        col = row.insertCell(i);
	 	col.innerHTML = '--';
	} 
	selectLine("record_no");
}

function removeInst(url)
{
   var rml = getElement('rml');
    if (rml == null)
	   return;
	   
    var SubmitForm = new webSubmitForm();	  
	var cnt = 0;  
    with (document.forms[0])
    {
        if (rml.length > 0)
        {
            for (var i = 0; i < rml.length; i++)
            {
                if (rml[i].checked == true)
                {
                    SubmitForm.addParameter(rml[i].value,'');
					cnt++;
                }
            }
        }
        else if (rml.checked == true)
        {
             SubmitForm.addParameter(rml.value,'');
			 cnt++;
        }
    }

   SubmitForm.setAction('del.cgi?RequestFile=' + url);   
   SubmitForm.submit();	
}

function writeTabInfoHeader(tabTitle, tabWidth, titleWidth)
{
	writeTabHeader(tabTitle,tabWidth,titleWidth,'info');
}

function writeTabCfgHeader(tabTitle, tabWidth,titleWidth)
{
	writeTabHeader(tabTitle,tabWidth,titleWidth,'cfg');
}

function writeTabTail()
{
    document.write("<\/td><\/tr><\/table>");
}



var previousTR = null;
function setLineHighLight(objTR)
{
    if (previousTR != null)
	{
	    previousTR.style.backgroundColor = '#f4f4f4';
		for (var i = 0; i < previousTR.cells.length; i++)
		{
			previousTR.cells[i].style.color = '#000000';
		}
	}
	
	objTR.style.backgroundColor = '#c7e7fe';
	for (var i = 0; i < objTR.cells.length; i++)
	{
		objTR.cells[i].style.color = '#000000';		
	}
	previousTR = objTR;
}
