function open_adguard()
{
    window.open("http://" + String(location.host) + ":3000" + "/");
}

function power_off()
{
    document.location.href = "/shutdown.html";
    var xhr = new XMLHttpRequest();
    xhr.open("POST", '/power-off', true);
    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhr.send("");
}

function reboot()
{
    document.location.href = "/reboot.html";
    var xhr = new XMLHttpRequest();
    xhr.open("POST", '/reboot', true);
    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhr.send("");
}

async function get_cpu_temp()
{
    let url = "/cpu-temp";
    let response = await fetch(url);
    if (response.ok) 
    {
        let temp = await response.text();
        alert(temp);
    } else {
        console.log("HTTP error: " + response.status);
    }
}

function download(link, filename) {
    var element = document.createElement('a');
    element.setAttribute('href', link);
  
    element.style.display = 'none';
    element.download = filename;
    document.body.appendChild(element);
  
    element.click();
  
    document.body.removeChild(element);
}

function download_config() 
{
    let select = document.getElementById("config-action");
    let user_choice = select.options[select.selectedIndex].text;
    let user_choice_index = select.selectedIndex;
    if (user_choice_index == 0) 
    {
        download("/config", "AdGuardHome.yaml");
    } 
    else if (user_choice_index == 1) 
    {
        uploadFile(document.getElementById("formFile"));
    }
}

function show_upload_form()
{
    let select = document.getElementById("config-action");
    let uploader = document.getElementById("uploader");
    let user_choice_index = select.selectedIndex;
    if (user_choice_index == 1)
    {
        uploader.style.display = "";
    } else {
        uploader.style.display = "none";
    }
}

function uploadFile(input) {
    let file = input.files[0];
    let reader = new FileReader();
    reader.readAsText(file);
    reader.onload = function() {
        update_config(reader.result);
    };
    
    reader.onerror = function() {
      console.log(reader.error);
    };
}

function go_root()
{
    location.href = "/";
}

function init_timer()
{
    setTimeout(go_root, 20000);
}

function update_config(new_config)
{
    var xhr = new XMLHttpRequest();
    xhr.open("POST", '/update-config', true);
    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhr.send(new_config);
}