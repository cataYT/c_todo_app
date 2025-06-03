# ONLY FOR WINDOWS

# Set script to stop on errors
$ErrorActionPreference = "Stop"

# === Configuration ===
$exeName = "todo_app.exe"
$sourceFiles = @("main.c", "todo.c")
$objFiles = $sourceFiles -replace "\.c$", ".obj"

# === Clean previous build artifacts ===
Write-Host "Cleaning previous builds..."
$pathsToDelete = @($exeName) + $objFiles
Remove-Item $pathsToDelete -ErrorAction SilentlyContinue

# === Compile with optimization flags ===
Write-Host "Compiling with optimization flags..."
foreach ($src in $sourceFiles) {
    cl /nologo /O2 /GL /Oi /Oy /Gy /MD /c $src
}

# === Link with full optimizations ===
Write-Host "Linking with LTCG and optimization..."
link /nologo /LTCG /OPT:REF /OPT:ICF /OUT:$exeName $objFiles

# === Done ===
Write-Host ""
Write-Host "Build successful: $exeName"