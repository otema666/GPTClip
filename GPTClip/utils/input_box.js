// JS navigator console to create a small box where user can write text
// This script creates a draggable, semi-transparent text input box on a webpage, allowing users to write text, minimize or close the box, and toggle its visibility using a keyboard shortcut (Ctrl + Shift + F).
let container = document.createElement("div");
container.style.position = "fixed";
container.style.top = "50%";
container.style.left = "50%";
container.style.width = "140px";
container.style.height = "100px";
container.style.opacity = "0.4";
container.style.backgroundColor = "rgba(0, 0, 0, 0.5)";
container.style.borderRadius = "10px";
container.style.marginTop = "-90px";
container.style.marginLeft = "-150px";
document.body.appendChild(container);

let titleBar = document.createElement("div");
titleBar.style.width = "100%";
titleBar.style.height = "30px";
titleBar.style.backgroundColor = "gray";
titleBar.style.cursor = "move";
titleBar.style.display = "flex";
titleBar.style.alignItems = "center";
titleBar.style.justifyContent = "flex-end";
titleBar.style.borderTopLeftRadius = "10px";
titleBar.style.borderTopRightRadius = "10px";
titleBar.style.boxShadow = "0 2px 5px rgba(0, 0, 0, 0.5)";

function createButton(label, onClick) {
    let buttonContainer = document.createElement("div");
    buttonContainer.style.display = "flex";
    buttonContainer.style.alignItems = "center";
    buttonContainer.style.marginRight = "10px";
    buttonContainer.style.borderRadius = "5px";
    buttonContainer.style.boxShadow = "0 2px 5px rgba(0, 0, 0, 0.3)";
    buttonContainer.style.overflow = "hidden";

    let button = document.createElement("button");
    button.innerText = label;
    button.style.width = "30px";
    button.style.height = "30px";
    button.style.backgroundColor = "transparent";
    button.style.border = "none";
    button.style.color = "white";
    button.style.cursor = "pointer";
    button.style.fontSize = "16px";
    button.style.padding = "0";
    button.onclick = onClick;

    button.onmouseover = () => button.style.color = "lightgray";
    button.onmouseout = () => button.style.color = "white";

    buttonContainer.appendChild(button);
    return buttonContainer;
}

let minimizeButton = createButton("-", function() {
    if (textArea.style.display === "none") {
        textArea.style.display = "block";
        container.style.height = "100px";
        container.style.opacity = "0.4";
    } else {
        textArea.style.display = "none";
        container.style.height = "30px";
        container.style.opacity = "0.2";
    }
});

let closeButton = createButton("X", function() {
    document.body.removeChild(container);
});

titleBar.appendChild(minimizeButton);
titleBar.appendChild(closeButton);

let textArea = document.createElement("textarea");
textArea.style.width = "100%";
textArea.style.height = "calc(100% - 30px)";
textArea.style.border = "none";
textArea.style.padding = "1px";
textArea.style.boxSizing = "border-box";
textArea.style.color = "black";
textArea.placeholder = "Escribe aquí...";
textArea.style.resize = "none";
textArea.style.outline = "none";
textArea.spellcheck = false;

container.appendChild(titleBar);
container.appendChild(textArea);

titleBar.onmousedown = function(event) {
    let shiftX = event.clientX - container.getBoundingClientRect().left;
    let shiftY = event.clientY - container.getBoundingClientRect().top;

    function moveAt(pageX, pageY) {
        container.style.left = pageX - shiftX + 'px';
        container.style.top = pageY - shiftY + 'px';
        container.style.marginLeft = "0";
        container.style.marginTop = "0";
    }

    function onMouseMove(event) {
        moveAt(event.pageX, event.pageY);
    }

    document.addEventListener('mousemove', onMouseMove);

    document.onmouseup = function() {
        document.removeEventListener('mousemove', onMouseMove);
        document.onmouseup = null;
    };
};

titleBar.ondragstart = function() {
    return false;
};

function toggleContainer() {
    if (document.body.contains(container)) {
        document.body.removeChild(container);
    } else {
        document.body.appendChild(container);
    }
}

document.addEventListener('keydown', function(event) {
    if (event.ctrlKey && event.shiftKey && event.key === 'F') {
        event.preventDefault();
        toggleContainer();
    }
});

container.onmouseenter = () => {
    if (textArea.style.display === "none") {
        container.style.opacity = "0.4";
    }
};

container.onmouseleave = () => {
    if (textArea.style.display === "none") {
        container.style.opacity = "0.2";
    }
};
