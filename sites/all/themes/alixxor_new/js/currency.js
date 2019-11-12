//var baseurl='http://alixxor.com';
var baseurl = 'http://d7.alixxor.com';

function showNewCurrencies()
{
    location.reload(true);
}

function update_query_param(field, value) {
    var current_url = document.location;
    var new_url = current_url.protocol + '//' + current_url.hostname + current_url.pathname + '?' + field + '=' + value;

    var query_string = current_url.search.substring(1);
    if (query_string.length) {
        var vars = query_string.split("&");
        for (var i = 0; i < vars.length; i++) {
            var pair = vars[i].split("=");
            if (pair[0] != field) {
                new_url += '&' + vars[i];
            }
        }
    }

    new_url += current_url.hash;
    window.location = new_url;
}

function change_currency(newccontrol) {
    var new_currency_element = document.getElementById("currencyselect"+newccontrol);
    var new_currency = new_currency_element.value;

    update_query_param('currency', new_currency);
}
