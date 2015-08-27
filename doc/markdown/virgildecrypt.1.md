NAME
====

**decrypt** -- decrypt data with given password or user's private key.

SYNOPSIS
========

**virgil decrypt** [--in *file*] [--out *file*] [--content-info *file*]
[--key *file* [--pwd *arg*]] --recipient *arg*

DESCRIPTION
===========

Decrypt data with given password or user's private key.

OPTIONS
=======

-i *file*, --in *file*  
Data to be decrypted. If omitted stdin is used.

-o *file*, --out *file*  
Decrypted data. If omitted stdout is used.

-c *file*, --content-info *file*  
Content info. Use this option to specify explicit content info if it not
embedded in the encrypted data.

-k *file*, --key *file*  
Recipient's private key.

-p *arg*, --pwd *arg*  
Recipient's private key password.

-r *arg*, --recipient *arg*  
(required) If option --key is defined this value is used as recipient's
Virgil Public Key, otherwise this value is used as recipient's password.

SEE ALSO
========

`virgil(1)`, `virgilencrypt(1)`