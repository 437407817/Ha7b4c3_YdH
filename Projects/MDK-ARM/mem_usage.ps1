# mem_usage.ps1 -- Keil 构建后内存用量可视化工具
# ============================================================
# 用法（Keil After Build 命令）:
#   "$P\mem_usage.bat"
#   脚本与 .uvprojx 放在同一目录（MDK-ARM\），无需传参
#   工程名自动从同目录 .uvprojx 文件名推断
#
# 也可直接调用（需手动指定 .map 路径）:
#   powershell -ExecutionPolicy Bypass -File "$P\mem_usage.ps1" "$P\工程名\工程名.map"
#
# 参数: .map 文件的完整路径
# 适用于任意 MCU Keil MDK 工程，无需 Python
# 需要 Windows PowerShell（Windows 10/11 内置）
# ============================================================

param(
    [string]$MapFile = ""
)

$BAR_WIDTH   = 24
$WARN_PCT    = 80
$CRIT_PCT    = 95
$LABEL_WIDTH = 11   # All bar labels right-padded to this width so '[' aligns
$SEP_WIDE    = "=" * 62
$SEP_THIN    = "-" * 62

# --- 颜色辅助函数 ---
function Write-Color([string]$Text, [string]$Color = "White") {
    Write-Host $Text -ForegroundColor $Color
}

function Write-Info ([string]$Text) { Write-Host $Text -ForegroundColor Cyan   }
function Write-Ok   ([string]$Text) { Write-Host $Text -ForegroundColor Green  }
function Write-Warn ([string]$Text) { Write-Host $Text -ForegroundColor Yellow }
function Write-Crit ([string]$Text) { Write-Host $Text -ForegroundColor Red    }

# --- 构建彩色进度条：已用=绿/黄/红，空间=深灰 ---
function Make-Bar([long]$Used, [long]$Total) {
    if ($Total -le 0) { $pct = 0.0 } else { $pct = $Used / $Total * 100 }
    $filled = [int]($BAR_WIDTH * $pct / 100)
    $empty  = $BAR_WIDTH - $filled
    $barColor = if ($pct -ge $CRIT_PCT) { "Red" } elseif ($pct -ge $WARN_PCT) { "Yellow" } else { "Green" }
    return $filled, $empty, $barColor, $pct
}

# 打印进度条行，标签对齐到 $LABEL_WIDTH 宽度以对齐 '['
function Write-Bar([string]$Label, [long]$Used, [long]$Total) {
    $r = Make-Bar $Used $Total
    $filled   = $r[0]; $empty = $r[1]; $barColor = $r[2]; $pct = $r[3]
    $usedStr  = Fmt-Size $Used
    $totalStr = Fmt-Size $Total
    $pctStr   = "{0,5:F1}%" -f $pct
    # 将标签右填充，使 '[' 始终从第 $LABEL_WIDTH + 2 列开始
    $paddedLabel = $Label.PadRight($LABEL_WIDTH)
    Write-Host "${paddedLabel}: " -NoNewline
    Write-Host "[" -NoNewline -ForegroundColor DarkGray
    Write-Host ("#" * $filled) -NoNewline -ForegroundColor $barColor
    Write-Host ("." * $empty)  -NoNewline -ForegroundColor DarkGray
    Write-Host "]" -NoNewline -ForegroundColor DarkGray
    Write-Host " $pctStr   $usedStr / $totalStr"
}

function Fmt-Size([long]$Bytes) {
    # 所有分支返回恰好 10 个字符，保持 '/' 列对齐
    if ($Bytes -ge 1048576) { return ("{0,7:F2} MB" -f ($Bytes / 1048576)) }  # "  x.xx MB" = 10
    if ($Bytes -ge 1024)    { return ("{0,7:F2} KB" -f ($Bytes / 1024)) }     # "  x.xx KB" = 10
    return (("{0} B" -f $Bytes).PadLeft(9) + " ")                             # "    xxx B " = 10
}

function Parse-Hex([string]$Hex) {
    $clean = $Hex -replace '0[xX]', ''
    return [Convert]::ToInt64($clean, 16)
}

# --- 检查 .map 文件参数 ---
if ($MapFile -eq "") {
    Write-Crit "[ERROR] Please provide .map file path as argument."
    Write-Warn "Usage: mem_usage.ps1 <path_to_map_file>"
    exit 1
}

# 解析完整路径（Resolve-Path 同时支持相对路径和绝对路径）
$RawInput = $MapFile
$MapFile  = Resolve-Path $MapFile -ErrorAction SilentlyContinue
if (-not $MapFile) {
    Write-Crit "[ERROR] .map file not found: $RawInput"
    Write-Warn "        Check that the project has been built at least once."
    exit 1
}

Write-Host ""
Write-Info  " > Analyzing: $MapFile"

$text = [System.IO.File]::ReadAllText($MapFile)

# --- 解析 Grand Totals ---
# AC5 格式: "  Code  (inc. data)  RO Data  RW Data  ZI Data  Debug   Name"
# AC6 格式: 列相同但可能有额外空白；均以 'Grand Totals' 结尾
# 正则: 'Grand Totals' 前出现 6+ 个数字列，捕获第 1,3,4,5 列（Code,RO,RW,ZI）
$m = [regex]::Match($text, '(?m)^\s*(\d+)\s+\d+\s+(\d+)\s+(\d+)\s+(\d+)\s+\d+\s+Grand Totals')
if (-not $m.Success) {
    # 备用：尝试 5 列变体（部分 AC6 .map 文件省略了 debug 列）
    $m = [regex]::Match($text, '(?m)^\s*(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+Grand Totals')
}
if (-not $m.Success) {
    Write-Crit "[ERROR] Grand Totals not found. Check .map file format (AC5/AC6)."
    exit 1
}
$code   = [long]$m.Groups[1].Value
$roData = [long]$m.Groups[2].Value
$rwData = [long]$m.Groups[3].Value
$ziData = [long]$m.Groups[4].Value

$romUsed = $code + $roData + $rwData
$ramUsed = $rwData + $ziData

# --- 解析 Load Region ---
$loadRx = 'Load Region ([\w\-]+)\s*\(Base:\s*(0x[\da-fA-F]+),\s*Size:\s*(0x[\da-fA-F]+),\s*Max:\s*(0x[\da-fA-F]+)'
$loadMatches = [regex]::Matches($text, $loadRx)

# --- 解析 Execution Region（含父 Load Region 名称）---
# 每个 Exec Region 连同其前置 Load Region 上下文一起捕获
$execRx = 'Execution Region ([\w\-]+)\s*\(Exec base:\s*(0x[\da-fA-F]+)[^)]*Size:\s*(0x[\da-fA-F]+),\s*Max:\s*(0x[\da-fA-F]+)'
$execMatches = [regex]::Matches($text, $execRx)

# 构建映射表: Load Region 名称 -> Execution Region 列表
# 策略: 按 Load Region 边界切分文本，再在各段中查找 Exec Region
$loadExecMap = @{}
for ($li = 0; $li -lt $loadMatches.Count; $li++) {
    $lStart = $loadMatches[$li].Index
    $lEnd   = if ($li + 1 -lt $loadMatches.Count) { $loadMatches[$li + 1].Index } else { $text.Length }
    $segment = $text.Substring($lStart, $lEnd - $lStart)
    $segExecs = [regex]::Matches($segment, $execRx)
    $loadExecMap[$loadMatches[$li].Groups[1].Value] = $segExecs
}

# --- 从 .map 文件结构自动推断 ROM/RAM 总容量 ---
# 策略: Load Region 对应 ROM（Flash 映像）
# Exec base 与 Load base 不同的 Exec Region = RAM（运行时映射到内存）
# 可适用于任意 Keil MDK MCU 工程，无需硬编码地址
$romMax = 0L
$ramMax = 0L

foreach ($lm in $loadMatches) {
    $lrBase = Parse-Hex $lm.Groups[2].Value
    $lrMax  = Parse-Hex $lm.Groups[4].Value
    # Load Region 本身 = ROM（Flash 映像）
    $romMax += $lrMax
}

foreach ($em in $execMatches) {
    $erBase = Parse-Hex $em.Groups[2].Value
    $erMax  = Parse-Hex $em.Groups[4].Value
    # 查找此 Exec Region 的父 Load Region base 地址
    $parentLoadBase = $null
    foreach ($lm in $loadMatches) {
        $relatedExecs = $loadExecMap[$lm.Groups[1].Value]
        if (-not $relatedExecs) { continue }
        foreach ($re in $relatedExecs) {
            if ($re.Groups[1].Value -eq $em.Groups[1].Value) {
                $parentLoadBase = Parse-Hex $lm.Groups[2].Value
                break
            }
        }
        if ($null -ne $parentLoadBase) { break }
    }
    # 若 exec base 与父 Load base 不同 -> 该 Region 居于 RAM
    if ($null -ne $parentLoadBase -and $erBase -ne $parentLoadBase) {
        $ramMax += $erMax
    }
}

if ($romMax -eq 0 -and $loadMatches.Count -gt 0) {
    $romMax = Parse-Hex $loadMatches[0].Groups[4].Value
}
if ($ramMax -eq 0) { $ramMax = 1 }

# --- 打印总计 ---
$romPct = if ($romMax -le 0) { 0.0 } else { $romUsed / $romMax * 100 }

# 计算各 RAM Region 使用率，取使用率最高的 Region 作为汇总行展示
# 避免多 Region 时总使用率被空 Region 稀释（如 SRAM1=90%，CCM=0% -> 总显示 56%）
$worstRamUsed = 0L
$worstRamMax  = 1L
foreach ($em in $execMatches) {
    $erBase = Parse-Hex $em.Groups[2].Value
    $erSz   = Parse-Hex $em.Groups[3].Value
    $erMx   = Parse-Hex $em.Groups[4].Value
    if ($erMx -le 0) { continue }
    $parentLoadBase = $null
    foreach ($lm in $loadMatches) {
        $relatedExecs = $loadExecMap[$lm.Groups[1].Value]
        if (-not $relatedExecs) { continue }
        foreach ($re in $relatedExecs) {
            if ($re.Groups[1].Value -eq $em.Groups[1].Value) {
                $parentLoadBase = Parse-Hex $lm.Groups[2].Value; break
            }
        }
        if ($null -ne $parentLoadBase) { break }
    }
    if ($null -ne $parentLoadBase -and $erBase -ne $parentLoadBase) {
        if ($erMx -gt 0 -and ($erSz / $erMx) -gt ($worstRamUsed / $worstRamMax)) {
            $worstRamUsed = $erSz
            $worstRamMax  = $erMx
        }
    }
}

Write-Host ""
Write-Color $SEP_WIDE DarkCyan
Write-Color "  * Total Memory Usage" White
Write-Color $SEP_WIDE DarkCyan
Write-Host ""
Write-Bar "  ROM" $romUsed $romMax
Write-Bar "  RAM" $worstRamUsed $worstRamMax
Write-Host ""

# --- 打印各分区详情 ---
Write-Color $SEP_WIDE DarkCyan
Write-Color "  * Section Detail" White
Write-Color $SEP_WIDE DarkCyan

foreach ($lm in $loadMatches) {
    $lrName = $lm.Groups[1].Value
    $lrBase = $lm.Groups[2].Value
    $lrSz   = Parse-Hex $lm.Groups[3].Value
    $lrMx   = Parse-Hex $lm.Groups[4].Value
    if ($lrMx -eq 0) { $lrMx = 1 }
    Write-Host ""
    Write-Host "  $lrName" -NoNewline -ForegroundColor White
    Write-Host "  ($lrBase)" -ForegroundColor DarkGray
    Write-Bar "    Load" $lrSz $lrMx

    # 只打印属于该 Load Region 的 Exec Region
    $relatedExecs = $loadExecMap[$lrName]
    if ($relatedExecs -and $relatedExecs.Count -gt 0) {
        for ($ei = 0; $ei -lt $relatedExecs.Count; $ei++) {
            $em     = $relatedExecs[$ei]
            $erName = $em.Groups[1].Value
            $erBase = $em.Groups[2].Value
            $erSz   = Parse-Hex $em.Groups[3].Value
            $erMx   = Parse-Hex $em.Groups[4].Value
            if ($erMx -eq 0) { $erMx = 1 }
            $erPct  = $erSz / $erMx * 100
            # 就地添加阈值标记（在 Keil 纯文本输出中也可辨识）
            $marker = if ($erPct -ge $CRIT_PCT) { " [X]" } elseif ($erPct -ge $WARN_PCT) { " [!]" } else { "" }
            Write-Host "    $erName$marker" -NoNewline -ForegroundColor Gray
            Write-Host "  ($erBase)" -ForegroundColor DarkGray
            Write-Bar "      Exec" $erSz $erMx
        }
    }
}

# --- 告警汇总 ---
# 仅对每个 Region 单独检查，不输出冗余的总体告警
$warnLines = @()

# 逐 Region 检查：超阈值则加入告警列表
foreach ($lm in $loadMatches) {
    $relatedExecs = $loadExecMap[$lm.Groups[1].Value]
    if (-not $relatedExecs) { continue }
    foreach ($em in $relatedExecs) {
        $erName = $em.Groups[1].Value
        $erSz   = Parse-Hex $em.Groups[3].Value
        $erMx   = Parse-Hex $em.Groups[4].Value
        if ($erMx -le 0) { continue }
        $erPct = $erSz / $erMx * 100
        if ($erPct -ge $CRIT_PCT) {
            $warnLines += ("[X] {0}: 使用率 {1:F1}%，已接近满载! (已用 {2} / 共 {3})" -f $erName, $erPct, (Fmt-Size $erSz).Trim(), (Fmt-Size $erMx).Trim())
        } elseif ($erPct -ge $WARN_PCT) {
            $warnLines += ("[!] {0}: 使用率 {1:F1}%，已超过 {2}% 预警阈值! (已用 {3} / 共 {4})" -f $erName, $erPct, $WARN_PCT, (Fmt-Size $erSz).Trim(), (Fmt-Size $erMx).Trim())
        }
    }
}

Write-Host ""
if ($warnLines.Count -gt 0) {
    Write-Color $SEP_THIN Yellow
    foreach ($line in $warnLines) {
        if ($line -match '^\[X\]') { Write-Crit "  $line" } else { Write-Warn "  $line" }
    }
    Write-Color $SEP_THIN Yellow
}

Write-Color $SEP_WIDE DarkCyan
Write-Host ""
