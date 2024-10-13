<?php
if (isset($_GET['prompt']) && isset($_GET['question'])) {
    $prompt = escapeshellarg($_GET['prompt']);
    $pregunta = escapeshellarg($_GET['question']);

    $command = "/var/www/html/gptClip/.venv/bin/python /var/www/html/gptClip/duckduckgo-chat-ai/gpt.py $prompt $pregunta";
    
    $output = shell_exec($command);
    header('Content-Type: text/plain; charset=utf-8');

    echo "$output";
}
?>