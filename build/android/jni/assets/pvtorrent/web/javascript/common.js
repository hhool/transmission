/**
 * Copyright © Dave Perrett and Malcolm Jarvis
 *
 * This file is licensed under the GPLv2.
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 */

var transmission,
    dialog,
    isMobileDevice = RegExp("(iPhone|iPod|Android)").test(navigator.userAgent),
    scroll_timeout;

if (!Array.indexOf){
	Array.prototype.indexOf = function(obj){
		var i, len;
		for (i=0, len=this.length; i<len; i++)
			if (this[i]==obj)
				return i;
		return -1;
	}
}

// http://forum.jquery.com/topic/combining-ui-dialog-and-tabs
$.fn.tabbedDialog = function (dialog_opts) {
	this.tabs({selected: 0});
	this.dialog(dialog_opts);
	this.find('.ui-tab-dialog-close').append(this.parent().find('.ui-dialog-titlebar-close'));
	this.find('.ui-tab-dialog-close').css({'position':'absolute','right':'0', 'top':'16px'});
	this.find('.ui-tab-dialog-close > a').css({'float':'none','padding':'0'});
	var tabul = this.find('ul:first');
	this.parent().addClass('ui-tabs').prepend(tabul).draggable('option','handle',tabul); 
	this.siblings('.ui-dialog-titlebar').remove();
	tabul.addClass('ui-dialog-titlebar');
}

$(document).ready(function() {

	// IE8 and below don’t support ES5 Date.now()
	if (!Date.now) {
		Date.now = function() {
			return +new Date();
		};
	}

	// IE specific fixes here
	if ($.browser.msie) {
		try {
			document.execCommand("BackgroundImageCache", false, true);
		} catch(err) {}
		$('.dialog_container').css('height',$(window).height()+'px');
	}

	if ($.browser.safari) {
		// Move search field's margin down for the styled input
		$('#torrent_search').css('margin-top', 3);
	}
	if (isMobileDevice){
		window.onload = function(){ setTimeout(function() { window.scrollTo(0,1); },500); };
		window.onorientationchange = function(){ setTimeout(function() { window.scrollTo(0,1); },100); };
		if (window.navigator.standalone)
			// Fix min height for isMobileDevice when run in full screen mode from home screen
			// so the footer appears in the right place
			$('body div#torrent_container').css('min-height', '338px');
		$("label[for=torrent_upload_url]").text("URL: ");
	} else {
		// Fix for non-Safari-3 browsers: dark borders to replace shadows.
		// Opera messes up the menu if we use a border on .trans_menu
		// div.outerbox so use ul instead
		$('.trans_menu ul, div#jqContextMenu, div.dialog_container div.dialog_window').css('border', '1px solid #777');
		// and this kills the border we used to have
		$('.trans_menu div.outerbox').css('border', 'none');
	}

	// Initialise the dialog controller
	dialog = new Dialog();

	// Initialise the main Transmission controller
	transmission = new Transmission();
});

/**
 * "innerHTML = html" is pretty slow in FF.  Happily a lot of our innerHTML
 * changes are triggered by periodic refreshes on torrents whose state hasn't
 * changed since the last update, so even this simple test helps a lot.
 */
function setInnerHTML(e, html)
{
	if (!e)
		return;

	/* innerHTML is listed as a string, but the browser seems to change it.
	 * For example, "&infin;" gets changed to "�? somewhere down the line.
	 * So, let's use an arbitrary  different field to test our state... */
	if (e.currentHTML != html)
	{
		e.currentHTML = html;
		e.innerHTML = html;
	}
};

/*
 *   Given a numerator and denominator, return a ratio string
 */
Math.ratio = function(numerator, denominator) {
	var result = Math.floor(100 * numerator / denominator) / 100;

	// check for special cases
	if (result==Number.POSITIVE_INFINITY || result==Number.NEGATIVE_INFINITY) result = -2;
	else if (isNaN(result)) result = -1;

	return result;
};

/**
 * Round a string of a number to a specified number of decimal places
 */
Number.prototype.toTruncFixed = function(place) {
	var ret = Math.floor(this * Math.pow (10, place)) / Math.pow(10, place);
	return ret.toFixed(place);
}

Number.prototype.toStringWithCommas = function() {
    return this.toString().replace(/\B(?=(?:\d{3})+(?!\d))/g, ",");
}


/*
 * Trim whitespace from a string
 */
String.prototype.trim = function () {
	return this.replace(/^\s*/, "").replace(/\s*$/, "");
}

/***
****  Preferences
***/

function Prefs() { }
Prefs.prototype = { };

Prefs._RefreshRate        = 'refresh_rate';

Prefs._FilterMode         = 'filter';
Prefs._FilterAll          = 'all';
Prefs._FilterActive       = 'active';
Prefs._FilterSeeding      = 'seeding';
Prefs._FilterDownloading  = 'downloading';
Prefs._FilterPaused       = 'paused';
Prefs._FilterFinished     = 'finished';

Prefs._SortDirection      = 'sort_direction';
Prefs._SortAscending      = 'ascending';
Prefs._SortDescending     = 'descending';

Prefs._SortMethod         = 'sort_method';
Prefs._SortByAge          = 'age';
Prefs._SortByActivity     = 'activity';
Prefs._SortByName         = 'name';
Prefs._SortByQueue        = 'queue_order';
Prefs._SortBySize         = 'size';
Prefs._SortByProgress     = 'percent_completed';
Prefs._SortByRatio        = 'ratio';
Prefs._SortByState        = 'state';

Prefs._CompactDisplayState= 'compact_display_state';

Prefs._Defaults =
{
	'filter': 'all',
	'refresh_rate' : 5,
	'sort_direction': 'ascending',
	'sort_method': 'name',
	'turtle-state' : false,
	'compact_display_state' : false
};

/*
 * Set a preference option
 */
Prefs.setValue = function(key, val)
{
	if (!(key in Prefs._Defaults))
		console.warn("unrecognized preference key '%s'", key);

	var days = 30;
	var date = new Date();
	date.setTime(date.getTime()+(days*24*60*60*1000));
	document.cookie = key+"="+val+"; expires="+date.toGMTString()+"; path=/";
};

/**
 * Get a preference option
 *
 * @param key the preference's key
 * @param fallback if the option isn't set, return this instead
 */
Prefs.getValue = function(key, fallback)
{
	var val;

	if (!(key in Prefs._Defaults))
		console.warn("unrecognized preference key '%s'", key);

	var lines = document.cookie.split(';');
	for (var i=0, len=lines.length; !val && i<len; ++i) {
		var line = lines[i].trim();
		var delim = line.indexOf('=');
		if ((delim === key.length) && line.indexOf(key) === 0)
			val = line.substring(delim + 1);
	}

	// FIXME: we support strings and booleans... add number support too?
	if (!val) val = fallback;
	else if (val === 'true') val = true;
	else if (val === 'false') val = false;
	return val;
};

/**
 * Get an object with all the Clutch preferences set
 *
 * @pararm o object to be populated (optional)
 */
Prefs.getClutchPrefs = function(o)
{
	if (!o)
		o = { };
	for (var key in Prefs._Defaults)
		o[key] = Prefs.getValue(key, Prefs._Defaults[key]);
	return o;
};


// forceNumeric() plug-in implementation
jQuery.fn.forceNumeric = function () {
	return this.each(function () {
		$(this).keydown(function (e) {
			var key = e.which || e.keyCode;
			return !e.shiftKey && !e.altKey && !e.ctrlKey &&
				// numbers   
				key >= 48 && key <= 57 ||
				// Numeric keypad
				key >= 96 && key <= 105 ||
				// comma, period and minus, . on keypad
				key === 190 || key === 188 || key === 109 || key === 110 ||
				// Backspace and Tab and Enter
				key === 8 || key === 9 || key === 13 ||
				// Home and End
				key === 35 || key === 36 ||
				// left and right arrows
				key === 37 || key === 39 ||
				// Del and Ins
				key === 46 || key === 45;
		});
	});
}


/**
 * http://blog.stevenlevithan.com/archives/parseuri
 *
 * parseUri 1.2.2
 * (c) Steven Levithan <stevenlevithan.com>
 * MIT License
 */
function parseUri (str) {
	var	o   = parseUri.options,
		m   = o.parser[o.strictMode ? "strict" : "loose"].exec(str),
		uri = {},
		i   = 14;

	while (i--) uri[o.key[i]] = m[i] || "";

	uri[o.q.name] = {};
	uri[o.key[12]].replace(o.q.parser, function ($0, $1, $2) {
		if ($1) uri[o.q.name][$1] = $2;
	});

	return uri;
};

parseUri.options = {
	strictMode: false,
	key: ["source","protocol","authority","userInfo","user","password","host","port","relative","path","directory","file","query","anchor"],
	q:   {
		name:   "queryKey",
		parser: /(?:^|&)([^&=]*)=?([^&]*)/g
	},
	parser: {
		strict: /^(?:([^:\/?#]+):)?(?:\/\/((?:(([^:@]*)(?::([^:@]*))?)?@)?([^:\/?#]*)(?::(\d*))?))?((((?:[^?#\/]*\/)*)([^?#]*))(?:\?([^#]*))?(?:#(.*))?)/,
		loose:  /^(?:(?![^:@]+:[^:@\/]*@)([^:\/?#.]+):)?(?:\/\/)?((?:(([^:@]*)(?::([^:@]*))?)?@)?([^:\/?#]*)(?::(\d*))?)(((\/(?:[^?#](?![^?#\/]*\.[^?#\/.]+(?:[?#]|$)))*\/?)?([^?#\/]*))(?:\?([^#]*))?(?:#(.*))?)/
	}
};

var base64EncodeChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
var base64DecodeChars = new Array( 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, 
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, 
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1); 

function base64encode(str) { 
    var out, i, len; 
    var c1, c2, c3; 

    len = str.length; 
    i = 0; 
    out = ""; 
    while(i < len) { 
    c1 = str.charCodeAt(i++) & 0xff; 
    if(i == len) 
    { 
        out += base64EncodeChars.charAt(c1 >> 2); 
        out += base64EncodeChars.charAt((c1 & 0x3) << 4); 
        out += "=="; 
        break; 
    } 
    c2 = str.charCodeAt(i++); 
    if(i == len) 
    { 
        out += base64EncodeChars.charAt(c1 >> 2); 
        out += base64EncodeChars.charAt(((c1 & 0x3)<< 4) | ((c2 & 0xF0) >> 4)); 
        out += base64EncodeChars.charAt((c2 & 0xF) << 2); 
        out += "="; 
        break; 
    } 
    c3 = str.charCodeAt(i++); 
    out += base64EncodeChars.charAt(c1 >> 2); 
    out += base64EncodeChars.charAt(((c1 & 0x3)<< 4) | ((c2 & 0xF0) >> 4)); 
    out += base64EncodeChars.charAt(((c2 & 0xF) << 2) | ((c3 & 0xC0) >>6)); 
    out += base64EncodeChars.charAt(c3 & 0x3F); 
    } 
    return out; 
} 

function base64decode(str) { 
    var c1, c2, c3, c4; 
    var i, len, out; 

    len = str.length; 
    i = 0; 
    out = ""; 
    while(i < len) { 
    /* c1 */ 
    do { 
        c1 = base64DecodeChars[str.charCodeAt(i++) & 0xff]; 
    } while(i < len && c1 == -1); 
    if(c1 == -1) 
        break; 

    /* c2 */ 
    do { 
        c2 = base64DecodeChars[str.charCodeAt(i++) & 0xff]; 
    } while(i < len && c2 == -1); 
    if(c2 == -1) 
        break; 

    out += String.fromCharCode((c1 << 2) | ((c2 & 0x30) >> 4)); 

    /* c3 */ 
    do { 
        c3 = str.charCodeAt(i++) & 0xff; 
        if(c3 == 61) 
        return out; 
        c3 = base64DecodeChars[c3]; 
    } while(i < len && c3 == -1); 
    if(c3 == -1) 
        break; 

    out += String.fromCharCode(((c2 & 0XF) << 4) | ((c3 & 0x3C) >> 2)); 

    /* c4 */ 
    do { 
        c4 = str.charCodeAt(i++) & 0xff; 
        if(c4 == 61) 
        return out; 
        c4 = base64DecodeChars[c4]; 
    } while(i < len && c4 == -1); 
    if(c4 == -1) 
        break; 
    out += String.fromCharCode(((c3 & 0x03) << 6) | c4); 
    } 
    return out; 
} 

function utf16to8(str) { 
    var out, i, len, c; 

    out = ""; 
    len = str.length; 
    for(i = 0; i < len; i++) { 
    c = str.charCodeAt(i); 
    if ((c >= 0x0001) && (c <= 0x007F)) { 
        out += str.charAt(i); 
    } else if (c > 0x07FF) { 
        out += String.fromCharCode(0xE0 | ((c >> 12) & 0x0F)); 
        out += String.fromCharCode(0x80 | ((c >> 6) & 0x3F)); 
        out += String.fromCharCode(0x80 | ((c >> 0) & 0x3F)); 
    } else { 
        out += String.fromCharCode(0xC0 | ((c >> 6) & 0x1F)); 
        out += String.fromCharCode(0x80 | ((c >> 0) & 0x3F)); 
    } 
    } 
    return out; 
} 

function utf8to16(str) { 
    var out, i, len, c; 
    var char2, char3; 

    out = ""; 
    len = str.length; 
    i = 0; 
    while(i < len) { 
    c = str.charCodeAt(i++); 
    switch(c >> 4) 
    { 
      case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: 
        // 0xxxxxxx 
        out += str.charAt(i-1); 
        break; 
      case 12: case 13: 
        // 110x xxxx 10xx xxxx 
        char2 = str.charCodeAt(i++); 
        out += String.fromCharCode(((c & 0x1F) << 6) | (char2 & 0x3F)); 
        break; 
      case 14: 
        // 1110 xxxx 10xx xxxx 10xx xxxx 
        char2 = str.charCodeAt(i++); 
        char3 = str.charCodeAt(i++); 
        out += String.fromCharCode(((c & 0x0F) << 12) | 
                       ((char2 & 0x3F) << 6) | 
                       ((char3 & 0x3F) << 0)); 
        break; 
    } 
    } 

    return out; 
} 


function doit() { 
    var f = document.f 
    f.output.value = base64encode(utf16to8(f.source.value)) 
    f.decode.value = utf8to16(base64decode(f.output.value)) 
} 
