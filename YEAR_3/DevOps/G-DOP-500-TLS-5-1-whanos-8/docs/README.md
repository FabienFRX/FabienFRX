# Whanos Documentation

This directory contains the complete Whanos documentation built with [mdBook](https://rust-lang.github.io/mdBook/).

## Quick Start

### View the Documentation

#### Option 1: Pre-built HTML (Fastest)

Simply open the pre-built documentation:

```bash
# Open in browser
open book/index.html

# Or use a simple HTTP server
cd book
python3 -m http.server 8000
# Then visit: http://localhost:8000
```

#### Option 2: mdBook Serve (Live Reload)

Install mdBook and serve with live reload:

```bash
# Install mdBook (if not installed)
cargo install mdbook

# Serve the documentation
mdbook serve

# Visit: http://localhost:3000
```

### Build the Documentation

To rebuild the documentation:

```bash
# Build
mdbook build

# Output will be in book/
```

### Clean Build

```bash
# Remove build artifacts
mdbook clean

# Rebuild
mdbook build
```

## Documentation Structure

```
docs/
├── book.toml           # mdBook configuration
├── src/                # Documentation source (Markdown)
│   ├── SUMMARY.md      # Table of contents
│   ├── introduction.md
│   ├── quick-start.md
│   ├── architecture/   # Architecture docs
│   ├── docker/         # Docker images docs
│   ├── jenkins/        # Jenkins configuration
│   ├── kubernetes/     # Kubernetes deployment
│   ├── user-guide/     # User guides
│   ├── admin/          # Administration
│   ├── troubleshooting/# Troubleshooting
│   ├── reference/      # Reference material
│   └── appendix/       # Appendices
└── book/               # Generated HTML (git ignored)
```

## Adding New Content

### Add a New Page

1. Create a new Markdown file in `src/`:
   ```bash
   nano src/new-page.md
   ```

2. Add entry to `src/SUMMARY.md`:
   ```markdown
   - [New Page](./new-page.md)
   ```

3. Rebuild:
   ```bash
   mdbook build
   ```

### Add a New Section

1. Create a directory in `src/`:
   ```bash
   mkdir src/new-section
   ```

2. Add pages in the directory

3. Update `src/SUMMARY.md`:
   ```markdown
   # New Section

   - [Overview](./new-section/overview.md)
   - [Details](./new-section/details.md)
   ```

## Configuration

Edit `book.toml` to customize:

- Title and description
- Theme (default: navy)
- Output options
- Search settings

See [mdBook Configuration](https://rust-lang.github.io/mdBook/format/configuration/index.html) for options.

## Tips

### Live Preview

Use `mdbook serve` for live preview while editing:

```bash
mdbook serve --open
```

Changes to Markdown files will auto-reload in the browser.

### Check Links

```bash
mdbook test
```

### Custom CSS

Add custom styles in `theme/custom.css` and reference in `book.toml`.

## Documentation Content Overview

| Section | Description | Status |
|---------|-------------|--------|
| Introduction | Project overview | ✅ Complete |
| Quick Start | 5-minute setup guide | ✅ Complete |
| Prerequisites | System requirements | ✅ Complete |
| Installation | Full installation guide | ✅ Complete |
| Architecture | System architecture | ✅ Complete |
| Docker Images | Base & standalone images | ✅ Complete |
| Jenkins | CI/CD configuration | ✅ Complete |
| Kubernetes | Cluster & deployment | ✅ Complete |
| User Guide | How to use Whanos | ✅ Complete |
| Admin | Infrastructure management | ✅ Complete |
| Troubleshooting | Problem solving | ✅ Complete |
| Reference | API & configuration | ✅ Complete |
| Appendix | FAQ, glossary, etc. | ✅ Complete |

## Building for Production

```bash
# Build optimized version
mdbook build

# The book/ directory contains the static site
# Deploy to any web server or hosting service

# Example: GitHub Pages
# 1. Copy book/ contents to gh-pages branch
# 2. Push to GitHub
# 3. Enable GitHub Pages in repository settings
```

## mdBook Installation

### Via Cargo (Rust)

```bash
cargo install mdbook
```

### Via Package Manager

**Ubuntu/Debian**:
```bash
# mdbook is not in default repos, use cargo
```

**macOS**:
```bash
brew install mdbook
```

### Download Binary

Download from [mdBook Releases](https://github.com/rust-lang/mdBook/releases)

## Troubleshooting

### mdbook command not found

```bash
# Ensure Cargo bin is in PATH
export PATH="$HOME/.cargo/bin:$PATH"

# Add to ~/.bashrc or ~/.zshrc
echo 'export PATH="$HOME/.cargo/bin:$PATH"' >> ~/.bashrc
```

### Build errors

```bash
# Check SUMMARY.md syntax
mdbook build --verbose

# Ensure all referenced files exist
```

### Missing pages

All files in `SUMMARY.md` must exist. Use `create-missing = true` in `book.toml` to auto-create.

## Contributing to Documentation

1. Fork the repository
2. Edit files in `docs/src/`
3. Test with `mdbook serve`
4. Submit pull request

See [Contributing](./src/appendix/contributing.md) for details.

## Additional Resources

- [mdBook Documentation](https://rust-lang.github.io/mdBook/)
- [Markdown Guide](https://www.markdownguide.org/)
- [Whanos GitHub Repository](https://github.com/your-org/whanos)

---

**Last Updated**: November 2025
