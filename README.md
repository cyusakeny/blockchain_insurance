# Health Insurance Blockchain System

A secure blockchain implementation for managing health insurance events including enrollments, payments, pre-authorizations, and claims processing.

## 📋 Project Description

This system implements a blockchain ledger for health insurance transactions with cryptographic security and data privacy features.

### Key Features

- **SHA-256 Hashing**: Ensures data integrity and immutability
- **Proof of Work**: Configurable mining difficulty for block validation
- **Health Insurance Events**: Supports enrollment, payment, pre-auth, claim submission, and claim decisions
- **Data Privacy**: Automatic masking of sensitive fields (member IDs, amounts, diagnosis codes)
- **Input Validation**: Comprehensive validation for all inputs
- **Persistence**: Save/load blockchain to disk
- **Chain Verification**: Integrity checking with cryptographic proof
- **Modular Architecture**: Clean code organization for maintainability

### Use Cases

- Insurance providers tracking policy lifecycles
- Healthcare providers submitting claims and pre-authorizations
- Auditors verifying transaction history
- Educational blockchain demonstrations

## 🚀 Getting Started

### Compilation

```bash
gcc -C main.c cli.c blockchain.c insurance_types.c sha256.c validation.c -o insurance_blockchain
```

### Running

```bash
./insurance_blockchain
```

## 📖 Usage

### Commands

| Command | Description |
|---------|-------------|
| `enroll` | Create new enrollment |
| `pay` | Record payment |
| `preauth` | Submit pre-authorization |
| `claim submit` | Submit claim |
| `claim decide` | Record claim decision |
| `view` | Display blockchain |
| `verify` | Verify integrity |
| `save` | Save to file |
| `load` | Load from file |
| `exit` | Save and exit |

### Example

```bash
> enroll
Enter Policy ID: POL001
Enter Member ID: MEM12345
Enter Provider ID: PROV789
Mining block 1...
Block mined!

> view
Member ID: MEM****45 (masked)
Amount: $**50.75 (masked)

> verify
Blockchain verified successfully!
```

## ⚠️ System Limitations and Known Issues

### Limitations

1. **Single-User System**: No authentication or role-based access control
2. **Data Masking Only**: Display masking, NOT encryption. Data stored unencrypted in files. **Not HIPAA-compliant**.
3. **Performance**: Sequential mining, no parallel processing. High difficulty = slow mining.
4. **No Networking**: Single-node only, no distributed consensus or P2P features
5. **Storage**: Binary format, no backup/redundancy, file corruption = data loss
6. **Input Constraints**: Max 32 chars for IDs, $1M limit on amounts, ASCII only

### Known Bugs

1. **File I/O**: Limited error handling, no atomic writes (corruption possible during save)
2. **Memory**: No leak detection, large chains (10K+ blocks) may cause issues
3. **Timestamps**: Relies on system clock (vulnerable to manipulation)
4. **Input Buffers**: Edge cases with scanf() may cause unexpected behavior

## 👥 Contributors

**Primary Contributor**: Cyusa Munezero Keny

**Built with C | Powered by Blockchain | Designed for Healthcare**