function OnDeleteButtonClick(tabTitle)
{
	var Selected = false;
	var RmlList = document.getElementsByName("rml");
    for (var i = 0; i < RmlList.length; i++)
	{
		if (RmlList[i].checked == true)
		{
			Selected = true;
		}
    }
	if (Selected == true)
	{
	  document.getElementById("DeleteButton").disabled = true;
	}

	clickRemove(tabTitle, "DeleteButton");
	

}
function writeTabHeader(tabTitle, width, titleWidth, type)
{
	if (width == null)
	    width = "70%";
		
	if (titleWidth == null)
	   titleWidth = "120";
			
	var html = 
			"<table width=\"" + width + "\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">"
			+ "<tr>"
			+ "<td>"
			+ "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">"
			+ "<tr>"
			+ " <td width=\"7\" height=\"\"> <\/td>"
			+ "<td valign=\"bottom\" align=\"center\" width=\"" + titleWidth + "\" >"
			+ "<\/td>"
			+ "<td width=\"7\"><\/td>"
			+ "<td align=\"right\">"
			+ "<table border=\"0\" cellpadding=\"1\" cellspacing=\"0\">"
			+ "<tr>";
	
	if ('info' == type)
	{
		 
	}
	else if ('cfg' == type)
	{
		html +=  '<td><input class="submit" id="Newbutton" type="button" value="New" '
		         + 'onclick="clickAdd(\''
		         + tabTitle + '\');"></td>'
				 
				 + '<td><input id="DeleteButton" class="submit" type="button" value="Delete" '
				 + 'onclick="OnDeleteButtonClick(\''
			     + tabTitle + '\');"></td>'
				 + '<td width="3"></td>';
	}
	
	html += "<\/tr>"
			+ "<\/table>"
			+ "<\/td>"
			+ "<\/tr>"
			+ "<\/table>"
			+ "<\/td>"
			+ "<\/tr>"
			+ "<tr>"
			+ "<td id=\"" + tabTitle + "\">";
	
	writeFile(html);			
	document.write(html);		
}
