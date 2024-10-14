// JS navigator console to make text selection more soft
// used in shortcut.c, line 42
document.head.appendChild(
    Object.assign(
        document.createElement('style'), 
        { 
            textContent: '::selection { background: rgba(0, 179, 255, 0.09); color: inherit; }' 
        }
    )
);
