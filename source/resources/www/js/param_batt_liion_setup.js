/**
 * @author SonLQ4
 */
 

 
function liionBattItem(id, vendor, serialNo) {
	this.id = id.trim();
	this.vendor = vendor.trim();
	this.serialNo = serialNo.trim();
}

function LIIONBATTStatusGet()
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
		
	
    var liionbatt = false;
    function liionbattComplete()
    {
		var LIIONBATTListInfo = new Array();
        if(liionbatt.readyState == 4)
        {
            if(liionbatt.status == 200)
            {                
				text = liionbatt.responseText;
				length = liionbatt.responseText.length;
				
				var info = text.split('@');
				if (info != '') {
					for ( i = 0; i < info.length; i++) {
						var name = info[i].split(';');
						if (name.length > 1) {
							LIIONBATTListInfo.push(new liionBattItem(name[0], name[1], name[2]));
						}
					}                                         
				}
				setRowText('idconfigliion_TotalNo', LIIONBATTListInfo.length);
				for (i = 0; i < LIIONBATTListInfo.length; i++)
				{		
					if(LIIONBATTListInfo[i].vendor == 15)
					{
						setRowText(LiionSetParam[0][i], '');
						setRowText(LiionSetParam[1][i], '');
						//setRowText(LiionSetParam[2][i], '');		
					
					} else
					{
						setRowText(LiionSetParam[0][i], LIIONBATTListInfo[i].id);
						
						if(LIIONBATTListInfo[i].vendor == 1)
						{
							setRowText(LiionSetParam[1][i], 'COSLIGHT');
						} 
						else if(LIIONBATTListInfo[i].vendor == 2)
						{
							setRowText(LiionSetParam[1][i], 'COSLIGHT_V1.1');
						} 
						else if(LIIONBATTListInfo[i].vendor == 3)
						{
							setRowText(LiionSetParam[1][i], 'SHOTO');
						} 
						else if(LIIONBATTListInfo[i].vendor == 4)
						{
							setRowText(LiionSetParam[1][i], 'HUAWEI');
						} 
						else if(LIIONBATTListInfo[i].vendor == 5)
						{
							setRowText(LiionSetParam[1][i], 'M1Viettel');
						} else if(LIIONBATTListInfo[i].vendor == 6)
						{
							setRowText(LiionSetParam[1][i], 'ZTT_2020');
						} else if(LIIONBATTListInfo[i].vendor == 7)
						{
							setRowText(LiionSetParam[1][i], 'ZTT');
						} else if(LIIONBATTListInfo[i].vendor == 8)
						{
							setRowText(LiionSetParam[1][i], 'HUAWEI_A1');
						} else if(LIIONBATTListInfo[i].vendor == 9)
						{
							setRowText(LiionSetParam[1][i], 'SAFT');
						} else if(LIIONBATTListInfo[i].vendor == 10)
						{
							setRowText(LiionSetParam[1][i], 'Narada');
						} else if(LIIONBATTListInfo[i].vendor == 11)
						{
							setRowText(LiionSetParam[1][i], 'EVE');
						} else if(LIIONBATTListInfo[i].vendor == 12)
						{
							setRowText(LiionSetParam[1][i], 'SHOTO_SDA10_48100');
						}else if(LIIONBATTListInfo[i].vendor == 13)
						{
							setRowText(LiionSetParam[1][i], 'COSLIGHT_CF4850T');
						}else if(LIIONBATTListInfo[i].vendor == 14)
						{
							setRowText(LiionSetParam[1][i], 'HUAFU_HF48100C');
						}						
						//setRowText(LiionSetParam[2][i], LIIONBATTListInfo[i].serialNo);						
					}	
				}						
			}			
        }
    }

    if(window.XMLHttpRequest)
    {
        liionbatt = new XMLHttpRequest();
    }
        else if(window.ActiveXObject)
    {
        liionbatt = new ActiveXObject("Microsoft.XMLHTTP");
    }
    if(liionbatt)
    {
        liionbatt.open("GET", "/liion_status?=", true);
        liionbatt.onreadystatechange = liionbattComplete;
        liionbatt.send(null);
    }
	t=setTimeout("LIIONBATTStatusGet()",5000);
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
			vPackVD1Set.open("GET", "/liionpack1v_value=" + vPackVD1DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD2Set.open("GET", "/liionpack2v_value=" + vPackVD2DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD3Set.open("GET", "/liionpack3v_value=" + vPackVD3DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD4Set.open("GET", "/liionpack4v_value=" + vPackVD4DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD5Set.open("GET", "/liionpack5v_value=" + vPackVD5DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD6Set.open("GET", "/liionpack6v_value=" + vPackVD6DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD7Set.open("GET", "/liionpack7v_value=" + vPackVD7DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD8Set.open("GET", "/liionpack8v_value=" + vPackVD8DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD9Set.open("GET", "/liionpack9v_value=" + vPackVD9DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD10Set.open("GET", "/liionpackAv_value=" + vPackVD10DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD11Set.open("GET", "/liionpackBv_value=" + vPackVD11DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD12Set.open("GET", "/liionpackCv_value=" + vPackVD12DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD13Set.open("GET", "/liionpackDv_value=" + vPackVD13DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD14Set.open("GET", "/liionpackEv_value=" + vPackVD14DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD15Set.open("GET", "/liionpackFv_value=" + vPackVD15DataSet.value + "&id=" + Math.random(), true);
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
			vPackVD16Set.open("GET", "/liionpackGv_value=" + vPackVD16DataSet.value + "&id=" + Math.random(), true);
			vPackVD16Set.send(null);
		}
	}
}


function fLiBTotalNumSet (){
	var vLiBTotalNumSet = false;
	var vLiBTotalNumDataSet = document.getElementById("idliion_TotalNo");
	
	if(window.XMLHttpRequest){
		vLiBTotalNumSet = new XMLHttpRequest();
	}
	else if(window.ActiveXObject){
		vLiBTotalNumSet = new ActiveXObject("Microsoft.XMLHTTP");
	}
	if(vLiBTotalNumSet)	{
		if(vLiBTotalNumDataSet.value != ""){
			vLiBTotalNumSet.open("GET", "/libTotal_value=" + vLiBTotalNumDataSet.value + "&id=" + Math.random(), true);
			vLiBTotalNumSet.send(null);
		}
	}
}

function LoadFrame() {

	LIIONBATTStatusGet();	
	
	LoadLanguageForTableTitle();
	LoadLanguageForSelection();	
}


