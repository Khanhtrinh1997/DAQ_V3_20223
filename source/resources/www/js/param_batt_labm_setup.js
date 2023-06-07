/**
 * @author SonLQ4
 */
 

 
function labmBattItem(id, vendor, serialNo) {
	this.id = id.trim();
	this.vendor = vendor.trim();
	this.serialNo = serialNo.trim();
}

function LABMStatusGet()
{
	var i=0;
	var j=0;
	var y=0;
	var x = 0;
	var t= 0;
	var m = 0;
	var length=0;
	var text;
	var temp1=Array(300);
	var temp2=Array(300);		
		
	
    var labmbatt = false;
    function labmbattComplete()
    {
		var LABMListInfo = new Array();
        if(labmbatt.readyState == 4)
        {
            if(labmbatt.status == 200)
            {                
				text = labmbatt.responseText;
				length = labmbatt.responseText.length;
				
				var info = text.split('@');
				if (info != '') {
					for ( i = 0; i < info.length; i++) {
						var name = info[i].split(';');
						if (name.length > 1) {
							LABMListInfo.push(new labmBattItem(name[0], name[1], name[2]));
						}
					}                                         
				}
				setRowText('idconfiglabm_TotalNo', LABMListInfo.length);
				for (i = 0; i < LABMListInfo.length; i++)
				{		
					if(LABMListInfo[i].vendor == 15)
					{
						setRowText(LabmSetParam[0][i], '');
						setRowText(LabmSetParam[1][i], '');
						//setRowText(LabmSetParam[2][i], '');		
					
					} else
					{
						setRowText(LabmSetParam[0][i], LABMListInfo[i].id);
						if(LABMListInfo[i].vendor == 1)
						{
							setRowText(LabmSetParam[1][i], 'M1Viettel');
						}
						//setRowText(LabmSetParam[2][i], LABMListInfo[i].serialNo);						
					}	
				}						
			}			
        }
    }

    if(window.XMLHttpRequest)
    {
        labmbatt = new XMLHttpRequest();
    }
        else if(window.ActiveXObject)
    {
        labmbatt = new ActiveXObject("Microsoft.XMLHTTP");
    }
    if(labmbatt)
    {
        labmbatt.open("GET", "/labm_status?=", true);
        labmbatt.onreadystatechange = labmbattComplete;
        labmbatt.send(null);
    }
	t=setTimeout("LABMStatusGet()",5000);
}

function fPackID1Set () {	
	
	var vPackVD1Set = false;
	var vPackVD1DataSet = document.getElementById("packvd1");

	if(window.XMLHttpRequest){		
		
		vPackVD1Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD1Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD1Set)	{
		if(vPackVD1DataSet.value != ""){
			vPackVD1Set.open("GET", "/labmpack1v_value=" + vPackVD1DataSet.value + "&id=" + Math.random(), true);
			vPackVD1Set.send(null);
		}
	}
}


function fPackID2Set () {	
	
	var vPackVD2Set = false;
	var vPackVD2DataSet = document.getElementById("packvd2");

	if(window.XMLHttpRequest){		
		
		vPackVD2Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD2Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD2Set)	{
		if(vPackVD2DataSet.value != ""){
			vPackVD2Set.open("GET", "/labmpack2v_value=" + vPackVD2DataSet.value + "&id=" + Math.random(), true);
			vPackVD2Set.send(null);
		}
	}
}

function fPackID3Set () {	
	
	var vPackVD3Set = false;
	var vPackVD3DataSet = document.getElementById("packvd3");

	if(window.XMLHttpRequest){		
		
		vPackVD3Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD3Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD3Set)	{
		if(vPackVD3DataSet.value != ""){
			vPackVD3Set.open("GET", "/labmpack3v_value=" + vPackVD3DataSet.value + "&id=" + Math.random(), true);
			vPackVD3Set.send(null);
		}
	}
}

function fPackID4Set () {	
	
	var vPackVD4Set = false;
	var vPackVD4DataSet = document.getElementById("packvd4");

	if(window.XMLHttpRequest){		
		
		vPackVD4Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD4Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD4Set)	{
		if(vPackVD4DataSet.value != ""){
			vPackVD4Set.open("GET", "/labmpack4v_value=" + vPackVD4DataSet.value + "&id=" + Math.random(), true);
			vPackVD4Set.send(null);
		}
	}
}

function fPackID5Set () {	
	
	var vPackVD5Set = false;
	var vPackVD5DataSet = document.getElementById("packvd5");

	if(window.XMLHttpRequest){		
		
		vPackVD5Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD5Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD5Set)	{
		if(vPackVD5DataSet.value != ""){
			vPackVD5Set.open("GET", "/labmpack5v_value=" + vPackVD5DataSet.value + "&id=" + Math.random(), true);
			vPackVD5Set.send(null);
		}
	}
}

function fPackID6Set () {	
	
	var vPackVD6Set = false;
	var vPackVD6DataSet = document.getElementById("packvd6");

	if(window.XMLHttpRequest){		
		
		vPackVD6Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD6Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD6Set)	{
		if(vPackVD6DataSet.value != ""){
			vPackVD6Set.open("GET", "/labmpack6v_value=" + vPackVD6DataSet.value + "&id=" + Math.random(), true);
			vPackVD6Set.send(null);
		}
	}
}

function fPackID7Set () {	
	
	var vPackVD7Set = false;
	var vPackVD7DataSet = document.getElementById("packvd7");

	if(window.XMLHttpRequest){		
		
		vPackVD7Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD7Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD7Set)	{
		if(vPackVD7DataSet.value != ""){
			vPackVD7Set.open("GET", "/labmpack7v_value=" + vPackVD7DataSet.value + "&id=" + Math.random(), true);
			vPackVD7Set.send(null);
		}
	}
}

function fPackID8Set () {	
	
	var vPackVD8Set = false;
	var vPackVD8DataSet = document.getElementById("packvd8");

	if(window.XMLHttpRequest){		
		
		vPackVD8Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD8Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD8Set)	{
		if(vPackVD8DataSet.value != ""){
			vPackVD8Set.open("GET", "/labmpack8v_value=" + vPackVD8DataSet.value + "&id=" + Math.random(), true);
			vPackVD8Set.send(null);
		}
	}
}

function fPackID9Set () {	
	
	var vPackVD9Set = false;
	var vPackVD9DataSet = document.getElementById("packvd9");

	if(window.XMLHttpRequest){		
		
		vPackVD9Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD9Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD9Set)	{
		if(vPackVD9DataSet.value != ""){
			vPackVD9Set.open("GET", "/labmpack9v_value=" + vPackVD9DataSet.value + "&id=" + Math.random(), true);
			vPackVD9Set.send(null);
		}
	}
}

function fPackID10Set () {	
	
	var vPackVD10Set = false;
	var vPackVD10DataSet = document.getElementById("packvd10");

	if(window.XMLHttpRequest){		
		
		vPackVD10Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD10Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD10Set)	{
		if(vPackVD10DataSet.value != ""){
			vPackVD10Set.open("GET", "/labmpackAv_value=" + vPackVD10DataSet.value + "&id=" + Math.random(), true);
			vPackVD10Set.send(null);
		}
	}
}

function fPackID11Set () {	
	
	var vPackVD11Set = false;
	var vPackVD11DataSet = document.getElementById("packvd11");

	if(window.XMLHttpRequest){		
		
		vPackVD11Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD11Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD11Set)	{
		if(vPackVD11DataSet.value != ""){
			vPackVD11Set.open("GET", "/labmpackBv_value=" + vPackVD11DataSet.value + "&id=" + Math.random(), true);
			vPackVD11Set.send(null);
		}
	}
}

function fPackID12Set () {	
	
	var vPackVD12Set = false;
	var vPackVD12DataSet = document.getElementById("packvd12");

	if(window.XMLHttpRequest){		
		
		vPackVD12Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD12Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD12Set)	{
		if(vPackVD12DataSet.value != ""){
			vPackVD12Set.open("GET", "/labmpackCv_value=" + vPackVD12DataSet.value + "&id=" + Math.random(), true);
			vPackVD12Set.send(null);
		}
	}
}

function fPackID13Set () {	
	
	var vPackVD13Set = false;
	var vPackVD13DataSet = document.getElementById("packvd13");

	if(window.XMLHttpRequest){		
		
		vPackVD13Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD13Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD13Set)	{
		if(vPackVD13DataSet.value != ""){
			vPackVD13Set.open("GET", "/labmpackDv_value=" + vPackVD13DataSet.value + "&id=" + Math.random(), true);
			vPackVD13Set.send(null);
		}
	}
}

function fPackID14Set () {	
	
	var vPackVD14Set = false;
	var vPackVD14DataSet = document.getElementById("packvd14");

	if(window.XMLHttpRequest){		
		
		vPackVD14Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD14Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD14Set)	{
		if(vPackVD14DataSet.value != ""){
			vPackVD14Set.open("GET", "/labmpackEv_value=" + vPackVD14DataSet.value + "&id=" + Math.random(), true);
			vPackVD14Set.send(null);
		}
	}
}

function fPackID15Set () {	
	
	var vPackVD15Set = false;
	var vPackVD15DataSet = document.getElementById("packvd15");

	if(window.XMLHttpRequest){		
		
		vPackVD15Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD15Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD15Set)	{
		if(vPackVD15DataSet.value != ""){
			vPackVD15Set.open("GET", "/labmpackFv_value=" + vPackVD15DataSet.value + "&id=" + Math.random(), true);
			vPackVD15Set.send(null);
		}
	}
}

function fPackID16Set () {	
	
	var vPackVD16Set = false;
	var vPackVD16DataSet = document.getElementById("packvd16");

	if(window.XMLHttpRequest){		
		
		vPackVD16Set = new XMLHttpRequest();	
	}
	else if(window.ActiveXObject){		
		vPackVD16Set = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	if(vPackVD16Set)	{
		if(vPackVD16DataSet.value != ""){
			vPackVD16Set.open("GET", "/labmpackGv_value=" + vPackVD16DataSet.value + "&id=" + Math.random(), true);
			vPackVD16Set.send(null);
		}
	}
}


function fLabmTotalNumSet (){
	var vLabmTotalNumSet = false;
	var vLabmTotalNumDataSet = document.getElementById("idlabm_TotalNo");
	
	if(window.XMLHttpRequest){
		vLabmTotalNumSet = new XMLHttpRequest();
	}
	else if(window.ActiveXObject){
		vLabmTotalNumSet = new ActiveXObject("Microsoft.XMLHTTP");
	}
	if(vLabmTotalNumSet)	{
		if(vLabmTotalNumDataSet.value != ""){
			vLabmTotalNumSet.open("GET", "/labmTotal_value=" + vLabmTotalNumDataSet.value + "&id=" + Math.random(), true);
			vLabmTotalNumSet.send(null);
		}
	}
}

function LoadFrame() {

	LABMStatusGet();	
	
	LoadLanguageForTableTitle();
	LoadLanguageForSelection();	
}


