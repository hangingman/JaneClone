$bakefile_url = 'https://github.com/vslavik/bakefile/releases/download/v1.2.5.1/bakefile-1.2.5.1_beta-win.zip'
 
iex ((new-object net.webclient).DownloadString($bakefile_url))
 
Write-Host "Waiting..."
[Console]::ReadKey() | Out-Null