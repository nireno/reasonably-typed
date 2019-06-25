const { execSync } = require('child_process');
let stdout = execSync('npm ls --json=true --depth=0');
let packageJson = JSON.parse(stdout);
let packages = Object.keys(packageJson.dependencies);
let packagesString = packages
  .map(package => `${package}@latest`)
  .join(' ');

console.log(`npm install ${packagesString}`);
