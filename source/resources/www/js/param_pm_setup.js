/**
 * @author SonLQ4
 */
 

 
function powermeterItem(id, vendor, serialNo) {
	this.id = id.trim();
	this.vendor = vendor.trim();
	this.serialNo = serialNo.trim();
}

function PMStatusGet()
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
		
	
    var powermeter = false;
    function powermeterComplete()
    {
		var PMListInfo = new Array();
        if(powermeter.readyState == 4)
        {
            if(powermeter.status == 200)
            {                
				text = powermeter.responseText;
				length = powermeter.responseText.length;
				
				var info = text.split('@');
				if (info != '') {
					for ( i = 0; i < info.length; i++) {
						var name = info[i].split(';');
						if (name.length > 1) {
							PMListInfo.push(new powermeterItem(name[0], name[1], name[2]));
						}
					}                                         
				}
				setRowText('idconfigpm_TotalNo', PMListInfo.length);
				for (i = 0; i < PMListInfo.length; i++)
				{		
					if(PMListInfo[i].vendor == 0)
					{
						setRowText(PMSetParam[0][i], '');
						setRowText(PMSetParam[1][i], '');
						/*setRowText(PMSetParam[2][i], '');*/		
					
					} else
					{
						setRowText(PMSetParam[0][i], PMListInfo[i].id);
						if(PMListInfo[i].vendor == 1)
						{
							setRowText(PMSetParam[1][i], 'FINECO');
						} else if(PMListInfo[i].vendor == 2)
						{
							setRowText(PMSetParam[1][i], 'ASCENT');
						}  else if(PMListInfo[i].vendor == 3)
						{
							setRowText(PMSetParam[1][i], 'EASTRON');
						}  else if(PMListInfo[i].vendor == 4)
						{
							setRowText(PMSetParam[1][i], 'CET1');
						} 	else if(PMListInfo[i].vendor == 5)	
						{
							setRowText(PMSetParam[1][i], 'PILOT');
						}	
						// else if(PMListInfo[i].vendor == 6)	
						// {
						// 	setRowText(PMSetParam[1][i], 'FORLONG');
						// }	
						else if(PMListInfo[i].vendor == 15)	
						{
							setRowText(PMSetParam[1][i], 'PILOT_3PHASE');
						}
						else if(PMListInfo[i].vendor == 16){
							setRowText(PMSetParam[1][i], 'YADA_3PHASE_DPC');
						}
						else if(PMListInfo[i].vendor == 17){
							setRowText(PMSetParam[1][i], 'Schneider 2022');
						}
						else if(PMListInfo[i].vendor == 18){
							setRowText(PMSetParam[1][i], 'EASTRON SMD72D 2022');
						}
						/*setRowText(PMSetParam[2][i], PMListInfo[i].serialNo);*/						
					}	
				}						
			}			
        }
    }

    if(window.XMLHttpRequest)
    {
        powermeter = new XMLHttpRequest();
    }
        else if(window.ActiveXObject)
    {
        powermeter = new ActiveXObject("Microsoft.XMLHTTP");
    }
    if(powermeter)
    {
        powermeter.open("GET", "/powermeter_status?=", true);
        powermeter.onreadystatechange = powermeterComplete;
        powermeter.send(null);
    }
	t=setTimeout("PMStatusGet()",5000);
}

function fPMID1Set () {	
	
	var vPMVD1Set = false;
	var vPMVD1DataSet = document.getElementById("pmvd1");

	if(window.XMLHttpRequest){		
		
		vPMVD1Set = new XMLHttpRequest();
	}
	else if(window.ActiveXObject){		
		
		vPMVD1Set = new ActiveXObject("Microsoft.XMLHTTP");
	}
	if(vPMVD1Set)	{
		if(vPMVD1DataSet.value != ""){
			vPMVD1Set.open("GET", "/pmnum1v_value=" + vPMVD1DataSet.value + "&id=" + Math.random(), true);
			vPMVD1Set.send(null);
		}
	}
}

function fPMTotalNumSet (){
	var vPMTotalNumSet = false;
	var vPMTotalNumDataSet = document.getElementById("idpm_TotalNo");
	
	if(window.XMLHttpRequest){
		vPMTotalNumSet = new XMLHttpRequest();
	}
	else if(window.ActiveXObject){
		vPMTotalNumSet = new ActiveXObject("Microsoft.XMLHTTP");
	}
	if(vPMTotalNumSet)	{
		if(vPMTotalNumDataSet.value != ""){
			vPMTotalNumSet.open("GET", "/pmTotal_value=" + vPMTotalNumDataSet.value + "&id=" + Math.random(), true);
			vPMTotalNumSet.send(null);
		}
	}
}

function LoadFrame() {

	PMStatusGet();	
	
	LoadLanguageForTableTitle();
	LoadLanguageForSelection();	
}


