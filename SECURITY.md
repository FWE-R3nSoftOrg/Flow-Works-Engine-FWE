# Security Policy

## Reporting a Vulnerability

If you find a security vulnerability in FWE — especially anything related to the license key system, ECC encryption, or file format parsing — **do not open a public issue**.

Report privately via GitHub's Security Advisory feature:
→ Security tab → "Report a vulnerability"

Include:
- Description of the vulnerability
- Steps to reproduce
- Potential impact
- Suggested fix if you have one

We'll respond within 72 hours and work with you on a coordinated disclosure.

## Scope

In scope:
- License key validation bypass
- ECC encryption weaknesses in `.fwe-*-ent` format
- File format parsing vulnerabilities (malformed zip, path traversal)
- IPC communication between sub-mains
- Panic function bypass

Out of scope:
- Third-party simulation backends (OpenFOAM, CalculiX, etc.) — report to them directly
- Issues requiring physical access to the machine
