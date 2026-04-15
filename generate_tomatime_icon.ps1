Add-Type -AssemblyName System.Drawing
$iconDir = 'asset\\icon'
if (-not (Test-Path $iconDir)) {
    New-Item -ItemType Directory -Path $iconDir | Out-Null
}
$iconPath = Join-Path $iconDir 'tomatime.ico'
$sourcePng = Join-Path $iconDir 'tomatime.png'

if (Test-Path $sourcePng) {
    try {
        $sourceBmp = [System.Drawing.Bitmap]::FromFile($sourcePng)
        $size = 64
        $bmp = New-Object System.Drawing.Bitmap $size, $size
        $g = [System.Drawing.Graphics]::FromImage($bmp)
        $g.Clear([System.Drawing.Color]::Transparent)
        $g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
        $g.DrawImage($sourceBmp, 0, 0, $size, $size)
        $g.Dispose()
        $sourceBmp.Dispose()

        $iconHandle = $bmp.GetHicon()
        $icon = [System.Drawing.Icon]::FromHandle($iconHandle)
        $fs = [System.IO.File]::OpenWrite($iconPath)
        $icon.Save($fs)
        $fs.Close()
        $bmp.Dispose()

        Write-Host "Created icon from source image: $iconPath"
        return
    } catch {
        Write-Warning "Failed to create icon from '$sourcePng': $_"
        Write-Host "Falling back to built-in tomato icon generation."
    }
}

$sizes = @(64, 32, 16)
$firstIcon = $null
foreach ($size in $sizes) {
    $bmp = New-Object System.Drawing.Bitmap $size, $size
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.Clear([System.Drawing.Color]::Transparent)

    $brush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 240, 64, 64))
    $pen = New-Object System.Drawing.Pen ([System.Drawing.Color]::Black, 2)
    $rect = [System.Drawing.Rectangle]::FromLTRB(4, 8, $size - 5, $size - 6)
    $g.FillEllipse($brush, $rect)
    $g.DrawEllipse($pen, $rect)

    $stemBrush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 50, 150, 30))
    $stem = [System.Drawing.Point[]]@(
        [System.Drawing.Point]([int]($size/2 - 8)), 6,
        [System.Drawing.Point]([int]($size/2 + 8)), 6,
        [System.Drawing.Point]([int]($size/2 + 12)), 14,
        [System.Drawing.Point]([int]($size/2 - 12)), 14
    )
    $g.FillPolygon($stemBrush, $stem)
    $g.DrawPolygon($pen, $stem)

    $leaf1 = [System.Drawing.Point[]]@(
        [System.Drawing.Point]([int]($size/2 - 12)), 14,
        [System.Drawing.Point]([int]($size/2 - 24)), 18,
        [System.Drawing.Point]([int]($size/2 - 10)), 22
    )
    $leaf2 = [System.Drawing.Point[]]@(
        [System.Drawing.Point]([int]($size/2 + 12)), 14,
        [System.Drawing.Point]([int]($size/2 + 26)), 16,
        [System.Drawing.Point]([int]($size/2 + 14)), 24
    )
    $g.FillPolygon($stemBrush, $leaf1)
    $g.FillPolygon($stemBrush, $leaf2)
    $g.DrawPolygon($pen, $leaf1)
    $g.DrawPolygon($pen, $leaf2)

    $g.Dispose()
    if (-not $firstIcon) {
        $firstIcon = $bmp
    }
}

$iconHandle = $firstIcon.GetHicon()
$icon = [System.Drawing.Icon]::FromHandle($iconHandle)
$fs = [System.IO.File]::OpenWrite($iconPath)
$icon.Save($fs)
$fs.Close()
Write-Host "Created icon: $iconPath"