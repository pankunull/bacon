<?php

function getClientIP() {
    $headers = [
        'HTTP_CF_CONNECTING_IP', // Cloudflare
        'HTTP_X_FORWARDED_FOR',
        'HTTP_X_REAL_IP',
        'REMOTE_ADDR'
    ];

    foreach ($headers as $key) {
        if (!empty($_SERVER[$key])) {
            // X-Forwarded-For can contain multiple IPs
            $ipList = explode(',', $_SERVER[$key]);
            return trim($ipList[0]);
        }
    }

    return 'UNKNOWN';
}

echo getClientIP();
