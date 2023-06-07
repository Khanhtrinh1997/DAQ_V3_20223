var ssidIdx = 0;
var changeWlanClick = 1;//初始化为2.4G信息页面
var WlanBasicPage = '2G';
var WlanAdvancePage = '2G';
var lanDevIndex = 1;
var QoSCurInterface = '';
var DDNSProvider = '';
var ripIndex = "";
var previousPage = "";
var preAddDomain = "";
var editIndex = -1;
var editDomain = '';
var curWebFrame = null;
var curWebMenuPath = null;
var productName = null; // 产品名称
var curLanguage = null; // 产品语言
var sptUserType = '1';
var sysUserType = '0';
var curUserType = null;
var MenuName = "";
var StartIndex = 1;
var Menu2Path = "";
var pageName11 = null;
var authMode = 0;
var Passwordmode = 0;
var changeMethod = 999;//初始化时根据当前语言判断
var UpgradeFlag = 0;
var SaveDataFlag = 0;
var menuArray = null;
var collectType = "";
var jumptomodifypwd = 0;
var PwdModifyFlag = 1;
var PwdAspNum = 0;
var SystemToolsNum = 0;
var EquipFlag = 0;
/**
 * Frame对象，包含对主页面的元素操作及内容页面的替换
 * @type Object
 */
var Frame = {
    menuItems : [],
    mainMenuCounter : 0,
    subMenuCounter : 0,
    $mainMenu : null,
    $subMenu : null,
    $content : null,
    /**
     * 页面初始化方法
     */
    init : function() {
        this.initData();
        this.initElement();
    },
    /**
     * 初始化数据，包含MENU元素的索引、页面元素的引用。
     */
    initData : function() {
    	var frame = this;
    	
        this.mainMenuCounter = 0;
        this.subMenuCounter = 0;

        this.$mainMenu = $("#headerTab ul");
        this.$subMenu = $("#nav ul");
        this.$content = $("#frameContent");
        
        // 当内容页面被加载时，重新设置页面的高度
        this.$content.load(function() {
        	frame.$content.show();
            frame.setContentHeight();
        });

        // 获取所有菜单项
        this.menuItems = eval(menuArray);
    },
    /**
     * 获取远程数据，方式为AJAX同步调用
     */
    getRemoteData : function() {
    },
    /**
     * 初始化页面元素
     */
	 
    initElement : function() {
    	$("#headerTitle").text(productName);
    	
    	this.setLogoutElement(curLanguage);
    	this.setCopyRightInfo(curLanguage);
    	
    	if (this.menuItems.length > 0) {
            this.addMenuItems(this.$mainMenu, this.menuItems, "main");
			
			if ((jumptomodifypwd == 0) && (curUserType != sysUserType) && (PwdModifyFlag == 1))
			{	
			    this.addMenuItems(this.$subMenu, this.menuItems[this.menuItems.length - 1].subMenus, "sub");
				var pwdurl = "html/management/account.asp";
				if(curLanguage.toUpperCase() == "ENGLISH")
				{
					MenuName = "System Tools";
					Menu2Path = "Modify Login Password";
				}
				else if(curLanguage.toUpperCase() == "PORTUGUESE")
				{
					MenuName = "Ferram. de Sis.";
					Menu2Path = "Alterar palavra-passe";
				}
				else if(curLanguage.toUpperCase() == "JAPANESE")
				{
					MenuName = "システムツール";
					Menu2Path = "ログインパスワードの変更";
				}
				else 
				{
					MenuName = "System Tools";
					Menu2Path = "Modify Login Password";
				}
				
				this.setContentPath(pwdurl);
			}
			else
			{	            
			    MenuName = this.menuItems[0].name;
                Menu2Path = this.menuItems[0].subMenus[0].name;
			    this.addMenuItems(this.$mainMenu, this.menuItems, "main");
                this.addMenuItems(this.$subMenu, this.menuItems[0].subMenus, "sub");
			}
        }
    },
    /**
     * 添加菜单项
     * @param {} element
     * @param {} menus
     * @param {} type
     */
    addMenuItems : function(element, menus, type) {
    	var frame = this;
		var posDeviceInfo = 0;
    	element.empty();
		if(type == "main") {
			this.mainMenuCounter = 0;
			for (var i = 0, len = menus.length; i < len; i++) {
				
				if( menus[i].name == "System Tools" || menus[i].name == "Ferram. de Sis." || menus[i].name == "システムツール")
				{
					SystemToolsNum = i;
				}
				
				element.append('<li value="' + i + '">' +
					'<div class="tabBtnLeft"></div>' +
					'<div class="tabBtnCenter">' + menus[i].name + '</div>' +
					'<div class="tabBtnRight"></div>' +
				'</li>');
		    }
			
			if ((jumptomodifypwd == 0)&& (curUserType != sysUserType) && (PwdModifyFlag == 1))
			{	
				element.children("li").eq(SystemToolsNum).addClass("hover");
			}
			else
			{
				 element.children("li").eq(0).addClass("hover");
			}
			
            element.children("li").click(function() {
                if (!element.children("li").eq(this.value).is(".hover")) {
					if(frame.mainMenuCounter != SystemToolsNum)
					{
						element.children("li").eq(SystemToolsNum).removeClass("hover");
					}
				
                    element.children("li").eq(frame.mainMenuCounter).removeClass("hover");
                    element.children("li").eq(this.value).addClass("hover");
                    frame.mainMenuCounter = this.value;
					MenuName = menus[this.value].name;
                    frame.addMenuItems($("#nav ul"), menus[this.value].subMenus, "sub");
                }
            });
		} else if (type == "sub") {
            this.subMenuCounter = 0;
            for (var i = 0, len = menus.length; i < len; i++) {
                element.append('<li value="' + i + '"><div>' + menus[i].name + '</div></li>');
				if("Device Information" == menus[i].name || "Infor. do dispositivo" == menus[i].name || "デバイス情報" == menus[i].name)
				{
					posDeviceInfo = i;
				}
				if(menus[i].name == "Modify Login Password" || menus[i].name == "Alterar palavra-passe" || menus[i].name == "ログインパスワードの変更" )
				{
					PwdAspNum = i;
				}
			}
			if ((jumptomodifypwd == 0) && (curUserType != sysUserType) && (PwdModifyFlag == 1))
			{	
				element.children("li").eq(PwdAspNum).addClass("hover");
				PwdModifyFlag = 0;
			}
			else
			{
            element.children("li").eq(frame.subMenuCounter).removeClass("hover");
			element.children("li").eq(posDeviceInfo).addClass("hover");
			frame.subMenuCounter = posDeviceInfo;
			Menu2Path = menus[posDeviceInfo].name;
			frame.setContentPath(menus[posDeviceInfo].url);
			}
            element.children("li").click(function() {
				 if(PwdAspNum != frame.subMenuCounter)
				 {
					 element.children("li").eq(PwdAspNum).removeClass("hover");
			     }
												 
                element.children("li").eq(frame.subMenuCounter).removeClass("hover");
                element.children("li").eq(this.value).addClass("hover");
                frame.subMenuCounter = this.value;

                Menu2Path = menus[this.value].name;
                frame.setContentPath(menus[this.value].url);
        	});
        }
    },
    /**
     * 设置内容页面的路径
     * @param {} url
     */
    setContentPath : function(url) {
		var msg;
		if (UpgradeFlag == 1)
		{
			if(curLanguage == 'english') {
				msg = 'Note: Upgrade will be interrupted and the device might not boot successfully next time if you switch to another page. It is strongly recommended to press cancel and remain on this page until the upgrade is complete.';
			}else if(curLanguage == 'portuguese') {
				msg = 'Nota: Actualização será interrompida e o dispositivo pode não iniciar com sucesso a próxima vez que mudar para outra página. É altamente recomendável pressionar cancelar e permanecer nesta página até a actualização estar concluída.';
			}else if(curLanguage == 'japanese') {
				msg = '備考: 別のページに切り替えると、アップグレードが中断され、次回デバイスがうまく起動できない可能性があります。 キャンセルを選択し、アップグレードが完了するまでこのページを切り替えないことをお薦めします。';
			}
			
			if(confirm(msg)) 
			{
				UpgradeFlag = 0;
				this.setMenuPath();
				this.$content.attr("src", url);
				//this.$content.attr('title', name);
			}
		}
		else 
		{
			this.setMenuPath();
			this.$content.attr("src", url);
			//this.$content.attr('title', name);
		}
    },
    /**
     * 设置内容页面的高度
     */
    setContentHeight : function() {
        setInterval(function() {
            try {
                var height = 0;
                // IE浏览器
                if (window.ActiveXObject) {
                    height = document.getElementById("frameContent").contentWindow.document.body.scrollHeight;
                }
                // 非IE浏览器
                else if (window.XMLHttpRequest) {
                    height = document.getElementById("frameContent").contentWindow.document.body.offsetHeight;
                }

                height = Math.max(height, 470);
                $("#center").height(height + 25);
                $("#nav").height(height + 25);
                $("#content").height(height + 25);
                $("#frameWarpContent").height(height);
                $("#frameContent").height(height);
            } catch (e) {

            }
        }, 200);
    },
    /**
     * 设置导航名称
     */
    setMenuPath : function() {
        $("#topNav #topNavMainMenu").text(MenuName);
        $("#topNav #topNavSubMenu").text(Menu2Path);
    },
    /**
     * 设置“Logout”元素的样式
     */
    setLogoutElement : function(curLanguage) {
    	if(curLanguage == "english")
		{
    		$("#headerLogout span").html("Logout");
    	} 
		else if(curLanguage == "portuguese")
		{
			$("#headerLogout span").html("Terminar sessão");
		}
		else if(curLanguage == "japanese")
		{
			$("#headerLogout span").html("ログアウト");
		}
    	var frame = this;
    	
        $("#headerLogout span").mouseover(function() {
            $("#headerLogout span").css({
                "color" : "#990000",
                "text-decoration" : "underline"
            });
        });
        $("#headerLogout span").mouseout(function() {
            $("#headerLogout span").css({
                "color" : "#000000",
                "text-decoration" : "none"
            });
        });
        $("#headerLogout span").click(function() {
			frame.clickLogout();
        });
    },
    /**
     * 设置“Copyright”信息
     * @param {} language
     */
	setCopyRightInfo : function(language) {
        if (language == "english") {
            $("#footerText").html("Copyright © Huawei Technologies Co., Ltd. 2009-2014. All rights reserved. ");
        }else if (language == "portuguese") {
            $("#footerText").html("Copyright © Huawei Technologies Co., Ltd 2009-2014. Todos os direitos reservados. ");
        }else if (language == "japanese") {
            $("#footerText").html("Copyright © Huawei Technologies Co., Ltd. 2009-2014. All rights reserved. ");
        }
    },
    /**
     * Logout事件函数
     */
    clickLogout : function() {
        var loc = '';
        loc += 'logout.cmd?';
        loc += 'sessionKey=' + sessionKey;
        var code = 'location="' + loc + '"';
        eval(code);
    },
	show : function() {
        var frame = this;
        frame.getRemoteData();
        $(document).ready(function() {
            frame.init();
        });
    },
	showjump : function(hpa,zpa) 
	{
		if((productName == 'HG8045') && (curUserType == sysUserType))
		{
			hpa--;
		}
		this.$mainMenu.children("li").eq(this.mainMenuCounter).removeClass("hover");
		this.$mainMenu.children("li").eq(this.menuItems.length-hpa).addClass("hover");
		this.mainMenuCounter=this.menuItems.length-hpa;
		this.addMenuItems($("#nav ul"), this.menuItems[this.menuItems.length-hpa].subMenus, "sub");
		this.$subMenu.children("li").eq(this.subMenuCounter).removeClass("hover");
		this.$subMenu.children("li").eq(zpa).addClass("hover");
		MenuName = this.menuItems[this.menuItems.length-hpa].name;
		Menu2Path = this.menuItems[this.menuItems.length-hpa].subMenus[zpa].name;
		this.setContentPath(this.menuItems[this.menuItems.length-hpa].subMenus[zpa].url);
		this.subMenuCounter=zpa;
	}
};

Frame.show();
