function changeColor(color){
    console.log(hex2rgb(color));
}

//returns a RGB string that is good to send to the python script
function hex2rgb(hexInput) {

    hexInput = hexInput.replace("#", "");

    var r = parseInt(hexInput.substring(0, 2), 16)
    var g = parseInt(hexInput.substring(2, 4), 16)
    var b = parseInt(hexInput.substring(4, 6), 16);

    return 'RGB,' + r + ',' + g + ',' + b;
}