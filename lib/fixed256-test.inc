static void fixed256_test_neg(void) {
  union Fixed256 got;
  {
    const union Fixed256 a = { .u64= { 0x91b7584a2265b1f5, 0xcd613e30d8f16adf, 0x1027c4d1c386bbc4, 0x00000000414c343c } };
    const union Fixed256 expect = { .u64= { 0x6e48a7b5dd9a4e0b, 0x329ec1cf270e9520, 0xefd83b2e3c79443b, 0xffffffffbeb3cbc3 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xc2ce6f447ed4d57b, 0x78e510617311d8a3, 0x612e7696a6cecc1b, 0x00000000c9e9c616 } };
    const union Fixed256 expect = { .u64= { 0x3d3190bb812b2a85, 0x871aef9e8cee275c, 0x9ed18969593133e4, 0xffffffff361639e9 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x9755d4c13a902931, 0x1a2b8f1ff1fd42a2, 0x51431193e6c3f339, 0x0000000007d4bedc } };
    const union Fixed256 expect = { .u64= { 0x68aa2b3ec56fd6cf, 0xe5d470e00e02bd5d, 0xaebcee6c193c0cc6, 0xfffffffff82b4123 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xc2cd789a380208a9, 0xf3c64af775a89294, 0xed2f89d94a2f20aa, 0x0000000005805975 } };
    const union Fixed256 expect = { .u64= { 0x3d328765c7fdf757, 0x0c39b5088a576d6b, 0x12d07626b5d0df55, 0xfffffffffa7fa68a } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x1622bd795fec898f, 0xa9ec0806705fca16, 0x1ba1621582283d15, 0x00000000c74803e3 } };
    const union Fixed256 expect = { .u64= { 0xe9dd4286a0137671, 0x5613f7f98fa035e9, 0xe45e9dea7dd7c2ea, 0xffffffff38b7fc1c } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xd707107e855c3844, 0x5eda92d864ac5db9, 0xbb968a437d5c8dfc, 0x0000000007923986 } };
    const union Fixed256 expect = { .u64= { 0x28f8ef817aa3c7bc, 0xa1256d279b53a246, 0x446975bc82a37203, 0xfffffffff86dc679 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa5ac06d864c2f2e3, 0x2b28fef02b9c014e, 0x3a1890c78092b4d4, 0x00000000fb695ffb } };
    const union Fixed256 expect = { .u64= { 0x5a53f9279b3d0d1d, 0xd4d7010fd463feb1, 0xc5e76f387f6d4b2b, 0xffffffff0496a004 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x33138131c541013d, 0xeb8ac8ce8a245e6b, 0x8c5fe8f8dc3bf364, 0x000000003b6fe507 } };
    const union Fixed256 expect = { .u64= { 0xccec7ece3abefec3, 0x1475373175dba194, 0x73a0170723c40c9b, 0xffffffffc4901af8 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa8c24d4244ef7feb, 0x9be3cecb8c497c68, 0xbab9f87ff5059285, 0x0000000001762741 } };
    const union Fixed256 expect = { .u64= { 0x573db2bdbb108015, 0x641c313473b68397, 0x454607800afa6d7a, 0xfffffffffe89d8be } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x69d495dd81355c53, 0xd037cdff7c240d49, 0x6a17b9af5b569643, 0x0000000058989008 } };
    const union Fixed256 expect = { .u64= { 0x962b6a227ecaa3ad, 0x2fc8320083dbf2b6, 0x95e84650a4a969bc, 0xffffffffa7676ff7 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xcdf8440407295e42, 0xa2a7ae1f3ac7652c, 0x8cfe5cd12d5db79b, 0x00000000959f3a51 } };
    const union Fixed256 expect = { .u64= { 0x3207bbfbf8d6a1be, 0x5d5851e0c5389ad3, 0x7301a32ed2a24864, 0xffffffff6a60c5ae } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x73f7ba8e0445d656, 0xc10faa4003ba33db, 0x47fc816ac16e2284, 0x000000003fe31d03 } };
    const union Fixed256 expect = { .u64= { 0x8c084571fbba29aa, 0x3ef055bffc45cc24, 0xb8037e953e91dd7b, 0xffffffffc01ce2fc } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xcc1b0c3e1c07724e, 0x2f429ce59ff3078f, 0x4a5012dc582c18c9, 0x0000000011cbc288 } };
    const union Fixed256 expect = { .u64= { 0x33e4f3c1e3f88db2, 0xd0bd631a600cf870, 0xb5afed23a7d3e736, 0xffffffffee343d77 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x4b63e0efb62ac1fe, 0xb3df44a47467537a, 0x7f1a355e526eb523, 0x0000000079490eab } };
    const union Fixed256 expect = { .u64= { 0xb49c1f1049d53e02, 0x4c20bb5b8b98ac85, 0x80e5caa1ad914adc, 0xffffffff86b6f154 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x4fdf8e1a060cea63, 0x57e54acc62f5680c, 0xcbd3f5e06bc15385, 0x000000003023580c } };
    const union Fixed256 expect = { .u64= { 0xb02071e5f9f3159d, 0xa81ab5339d0a97f3, 0x342c0a1f943eac7a, 0xffffffffcfdca7f3 } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x6e80fa489b0bca16, 0xf9bddea5d12982e4, 0x39b21c95055455e8, 0x000000000492c4f5 } };
    const union Fixed256 expect = { .u64= { 0x917f05b764f435ea, 0x0642215a2ed67d1b, 0xc64de36afaabaa17, 0xfffffffffb6d3b0a } };
    Fixed256_neg(&got, &a);
    CHECK(got, expect);
  }
}

static void fixed256_test_square(void) {
  union Fixed256 got;
  {
    const union Fixed256 a = { .u64= { 0xdcf4bb99f4bea973, 0xd95bafc8f2a4d27b, 0x177219d30e7a269f, 0x0000000015ba2bdd } };
    const union Fixed256 expect = { .u64= { 0xeef65a841cc2a120, 0x564f2e35af8608b3, 0xf8e4449950ff05a4, 0x01d812960aa2cedb } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x2b491044d5e34124, 0xcf1822ffbc688778, 0xda94e3e8ab73738f, 0x000000004ee207f8 } };
    const union Fixed256 expect = { .u64= { 0x7e1a5f82aa493240, 0xb108a4ca80e9a8a2, 0x8edb4930c00b71ba, 0x184e846db8d437a0 } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x6c71c4a66148a86f, 0xe2520e33e44c5055, 0x2a1be9cd8697bbd0, 0x000000008f7d9b78 } };
    const union Fixed256 expect = { .u64= { 0x62657501ec7593c0, 0x93d54069f5a9e216, 0x3d4a316d498fa786, 0x506d915170cf09db } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa7a83ee0761ebfd2, 0x3ff98ff387c56473, 0x47733e847d718d73, 0x00000000ecc1cb63 } };
    const union Fixed256 expect = { .u64= { 0x898dfa562f1f5c72, 0xf2f5e8ecc30e582d, 0xa7481bfd99c4e81c, 0xdaf5e1b35904d0d9 } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x83f0be4e80371eb9, 0xcbd4d3e2d4dec9ef, 0xa9643a295a9ac6de, 0x00000000e202849d } };
    const union Fixed256 expect = { .u64= { 0x99183329728247e2, 0x1be296ee2c8531a4, 0x7d9e29bd51728c23, 0xc788722cb63e940a } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x8feb994a81167346, 0x81e220df848b1df7, 0x9da59b74a6c3181c, 0x0000000096838b76 } };
    const union Fixed256 expect = { .u64= { 0x992132b33551ffae, 0xb8ee765cd2644912, 0x595fd45c84bbd56e, 0x587e6b07081d6fd1 } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xe1cf4f589f8e4ce0, 0xc8c69069134bccd3, 0x5769da05d205bbfc, 0x00000000b9d7d01f } };
    const union Fixed256 expect = { .u64= { 0xd4d5b1703562a9e2, 0x63a89f91a3cbab20, 0xeb186ffbd2daa007, 0x86e9a0bc8a2ec2ca } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x0e8a35b10828d569, 0x5c3747465cc36c27, 0x3fdf57cd2c006497, 0x00000000ac3a5b26 } };
    const union Fixed256 expect = { .u64= { 0x1e137d18f47caad0, 0x0943cbc636958e2d, 0x4ac2159b61597bdb, 0x73de77c8d39d464f } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa0b4a2969d8055a9, 0xbf67da14be11d56b, 0x49390aa51cf5192b, 0x000000005653a45c } };
    const union Fixed256 expect = { .u64= { 0x7ebf71bdd6f380fe, 0xa10b6986b6a71238, 0xa21a3c6677de06cd, 0x1d1c4dc1fe902d39 } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x4ef25c0707e33868, 0x8d2f527e72daf0a5, 0x9aeb7fa0c4169b14, 0x00000000bd6d2500 } };
    const union Fixed256 expect = { .u64= { 0xc5e9c64815f8fd28, 0x3508a455cfe859a9, 0x8115031bed802684, 0x8c2a572b6c9cd3c6 } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x4391b6e2e6eacb0f, 0x66dfe717c1731339, 0x9f20dbb0dcc93f0e, 0x00000000b4917fc0 } };
    const union Fixed256 expect = { .u64= { 0x56873e7c01bdfc4e, 0xf47ee0654e0747b6, 0x4301f61de46b88dd, 0x7f5cee58d7bb26fb } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xb310b730049dd332, 0xf58105748ed5d1b7, 0xaba9e7b823fc5ad2, 0x000000000e893302 } };
    const union Fixed256 expect = { .u64= { 0x503aba0f722d767f, 0xbecd8c1145ce01e9, 0xfb1d879327b6d3b8, 0x00d34b1bedcc619e } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x21b7379f0897246a, 0x2bb3b36f29421c40, 0x74115c86188b1044, 0x00000000a294523d } };
    const union Fixed256 expect = { .u64= { 0x041813f8524e71d6, 0xb39f2772c6ee4b37, 0x56481e08d0515473, 0x67400e04f85f9a48 } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xea990e94821d4606, 0xb5459d83fbc46f1a, 0x0809eae3ef232a32, 0x00000000fe9934ad } };
    const union Fixed256 expect = { .u64= { 0x21f3eecde0843633, 0xdb3a8f9d9c0e1352, 0xf812500d297cf32f, 0xfd3460375d9e0a2e } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xb6c004cc3b8367dc, 0x12d4050771d7b14e, 0x1497024640332b06, 0x00000000975b54a3 } };
    const union Fixed256 expect = { .u64= { 0x021db1bd1410f0d9, 0x25bf42f5583d4fce, 0xd120f227bed92e1f, 0x597cde6daa363570 } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x475b51096c4ad652, 0xc02c6b9586b4625b, 0x26af8090013c3273, 0x0000000009167d41 } };
    const union Fixed256 expect = { .u64= { 0xab591ac99cb12d47, 0xe64673029209e53c, 0x475aeeb5ff0eb12a, 0x005296c85933ad90 } };
    Fixed256_square(&got, &a);
    CHECK(got, expect);
  }
}

static void fixed256_test_add(void) {
  union Fixed256 got;
  {
    const union Fixed256 a = { .u64= { 0x91b7584a2265b1f5, 0xcd613e30d8f16adf, 0x1027c4d1c386bbc4, 0x00000000414c343c } };
    const union Fixed256 b = { .u64= { 0xc2ce6f447ed4d57b, 0x78e510617311d8a3, 0x612e7696a6cecc1b, 0x00000000c9e9c616 } };
    const union Fixed256 expect = { .u64= { 0x5485c78ea13a8770, 0x46464e924c034383, 0x71563b686a5587e0, 0x000000010b35fa52 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x9755d4c13a902931, 0x1a2b8f1ff1fd42a2, 0x51431193e6c3f339, 0x0000000007d4bedc } };
    const union Fixed256 b = { .u64= { 0xc2cd789a380208a9, 0xf3c64af775a89294, 0xed2f89d94a2f20aa, 0x0000000005805975 } };
    const union Fixed256 expect = { .u64= { 0x5a234d5b729231da, 0x0df1da1767a5d537, 0x3e729b6d30f313e4, 0x000000000d551852 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x1622bd795fec898f, 0xa9ec0806705fca16, 0x1ba1621582283d15, 0x00000000c74803e3 } };
    const union Fixed256 b = { .u64= { 0xd707107e855c3844, 0x5eda92d864ac5db9, 0xbb968a437d5c8dfc, 0x0000000007923986 } };
    const union Fixed256 expect = { .u64= { 0xed29cdf7e548c1d3, 0x08c69aded50c27cf, 0xd737ec58ff84cb12, 0x00000000ceda3d69 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa5ac06d864c2f2e3, 0x2b28fef02b9c014e, 0x3a1890c78092b4d4, 0x00000000fb695ffb } };
    const union Fixed256 b = { .u64= { 0x33138131c541013d, 0xeb8ac8ce8a245e6b, 0x8c5fe8f8dc3bf364, 0x000000003b6fe507 } };
    const union Fixed256 expect = { .u64= { 0xd8bf880a2a03f420, 0x16b3c7beb5c05fb9, 0xc67879c05ccea839, 0x0000000136d94502 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa8c24d4244ef7feb, 0x9be3cecb8c497c68, 0xbab9f87ff5059285, 0x0000000001762741 } };
    const union Fixed256 b = { .u64= { 0x69d495dd81355c53, 0xd037cdff7c240d49, 0x6a17b9af5b569643, 0x0000000058989008 } };
    const union Fixed256 expect = { .u64= { 0x1296e31fc624dc3e, 0x6c1b9ccb086d89b2, 0x24d1b22f505c28c9, 0x000000005a0eb74a } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xcdf8440407295e42, 0xa2a7ae1f3ac7652c, 0x8cfe5cd12d5db79b, 0x00000000959f3a51 } };
    const union Fixed256 b = { .u64= { 0x73f7ba8e0445d656, 0xc10faa4003ba33db, 0x47fc816ac16e2284, 0x000000003fe31d03 } };
    const union Fixed256 expect = { .u64= { 0x41effe920b6f3498, 0x63b7585f3e819908, 0xd4fade3beecbda20, 0x00000000d5825754 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xcc1b0c3e1c07724e, 0x2f429ce59ff3078f, 0x4a5012dc582c18c9, 0x0000000011cbc288 } };
    const union Fixed256 b = { .u64= { 0x4b63e0efb62ac1fe, 0xb3df44a47467537a, 0x7f1a355e526eb523, 0x0000000079490eab } };
    const union Fixed256 expect = { .u64= { 0x177eed2dd232344c, 0xe321e18a145a5b0a, 0xc96a483aaa9acdec, 0x000000008b14d133 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x4fdf8e1a060cea63, 0x57e54acc62f5680c, 0xcbd3f5e06bc15385, 0x000000003023580c } };
    const union Fixed256 b = { .u64= { 0x6e80fa489b0bca16, 0xf9bddea5d12982e4, 0x39b21c95055455e8, 0x000000000492c4f5 } };
    const union Fixed256 expect = { .u64= { 0xbe608862a118b479, 0x51a32972341eeaf0, 0x058612757115a96e, 0x0000000034b61d02 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xb1eedaffcc3d5506, 0x736a947a843fdda7, 0x861e02ec39235bc0, 0x00000000a6048457 } };
    const union Fixed256 b = { .u64= { 0xacc66a576518093d, 0xcdaaac43936aa40c, 0xa8ea37f7523d2a54, 0x00000000a185cc8e } };
    const union Fixed256 expect = { .u64= { 0x5eb5455731555e43, 0x411540be17aa81b4, 0x2f083ae38b608615, 0x00000001478a50e6 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xbcc99ae80f0c8a89, 0x202cc8284c717095, 0x364e433ff7c882f4, 0x00000000e023033d } };
    const union Fixed256 b = { .u64= { 0x121b28004e6f5a94, 0x1391f9b9dbc799b0, 0xeacc110e4f73fd94, 0x00000000f07534fe } };
    const union Fixed256 expect = { .u64= { 0xcee4c2e85d7be51d, 0x33bec1e228390a45, 0x211a544e473c8088, 0x00000001d098383c } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x9f7a7dafb43adc4f, 0x0994940e82458cc8, 0x334de73d60c290d0, 0x0000000058d07674 } };
    const union Fixed256 b = { .u64= { 0x92c9357d34accd78, 0xe585552fac954ab5, 0x976699cc6ed5d1bf, 0x0000000031b1c27e } };
    const union Fixed256 expect = { .u64= { 0x3243b32ce8e7a9c7, 0xef19e93e2edad77e, 0xcab48109cf98628f, 0x000000008a8238f2 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xf01dbf291abb8ba3, 0x63db01fcaa7c314b, 0x810d2e304bcb6b22, 0x000000007ff2e341 } };
    const union Fixed256 b = { .u64= { 0x9cb471a55349da48, 0x66fec086df229650, 0x4806aa81e65150b5, 0x0000000004a1bde4 } };
    const union Fixed256 expect = { .u64= { 0x8cd230ce6e0565eb, 0xcad9c283899ec79c, 0xc913d8b2321cbbd7, 0x000000008494a125 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xdb87872d336b1a45, 0xcfa6cf3e53e6d093, 0x903715c8fcaf4a5a, 0x00000000c85f0d46 } };
    const union Fixed256 b = { .u64= { 0x6de2b33b56cef8ec, 0x443baac536891eeb, 0x18ae013eaca91679, 0x00000000d67393d6 } };
    const union Fixed256 expect = { .u64= { 0x496a3a688a3a1331, 0x13e27a038a6fef7f, 0xa8e51707a95860d4, 0x000000019ed2a11c } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xc49872c67c081bb7, 0x88534206fc4a447e, 0x10b8fe223c116549, 0x00000000b9b81635 } };
    const union Fixed256 b = { .u64= { 0x220d672b15ad9a9d, 0x2aa3300b2b711343, 0x89c80c4de9367ed9, 0x000000003685156b } };
    const union Fixed256 expect = { .u64= { 0xe6a5d9f191b5b654, 0xb2f6721227bb57c1, 0x9a810a702547e422, 0x00000000f03d2ba0 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x550d40ddc2557035, 0x8181e84d99a74924, 0x415ac400d7547080, 0x000000005e3c536c } };
    const union Fixed256 b = { .u64= { 0x1d43d1ffecd1345e, 0xe592067375305db7, 0x1b943cfc46f57327, 0x00000000c979cb06 } };
    const union Fixed256 expect = { .u64= { 0x725112ddaf26a493, 0x6713eec10ed7a6db, 0x5cef00fd1e49e3a8, 0x0000000127b61e72 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x4bb57b5cd3e89d32, 0x9d19ee45032b7328, 0x03b96d91aba018ea, 0x0000000017788b95 } };
    const union Fixed256 b = { .u64= { 0xb62c228e40df7c9a, 0x50806f017a1d556c, 0x35263b4519a2105c, 0x00000000a6ecc31f } };
    const union Fixed256 expect = { .u64= { 0x01e19deb14c819cc, 0xed9a5d467d48c895, 0x38dfa8d6c5422946, 0x00000000be654eb4 } };
    Fixed256_add(&got, &a, &b);
    CHECK(got, expect);
  }
}

static void fixed256_test_sub(void) {
  union Fixed256 got;
  {
    const union Fixed256 a = { .u64= { 0xdcf4bb99f4bea973, 0xd95bafc8f2a4d27b, 0x177219d30e7a269f, 0x0000000015ba2bdd } };
    const union Fixed256 b = { .u64= { 0x2b491044d5e34124, 0xcf1822ffbc688778, 0xda94e3e8ab73738f, 0x000000004ee207f8 } };
    const union Fixed256 expect = { .u64= { 0xb1abab551edb684f, 0x0a438cc9363c4b03, 0x3cdd35ea6306b310, 0xffffffffc6d823e4 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x6c71c4a66148a86f, 0xe2520e33e44c5055, 0x2a1be9cd8697bbd0, 0x000000008f7d9b78 } };
    const union Fixed256 b = { .u64= { 0xa7a83ee0761ebfd2, 0x3ff98ff387c56473, 0x47733e847d718d73, 0x00000000ecc1cb63 } };
    const union Fixed256 expect = { .u64= { 0xc4c985c5eb29e89d, 0xa2587e405c86ebe1, 0xe2a8ab4909262e5d, 0xffffffffa2bbd014 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x83f0be4e80371eb9, 0xcbd4d3e2d4dec9ef, 0xa9643a295a9ac6de, 0x00000000e202849d } };
    const union Fixed256 b = { .u64= { 0x8feb994a81167346, 0x81e220df848b1df7, 0x9da59b74a6c3181c, 0x0000000096838b76 } };
    const union Fixed256 expect = { .u64= { 0xf4052503ff20ab73, 0x49f2b3035053abf7, 0x0bbe9eb4b3d7aec2, 0x000000004b7ef927 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xe1cf4f589f8e4ce0, 0xc8c69069134bccd3, 0x5769da05d205bbfc, 0x00000000b9d7d01f } };
    const union Fixed256 b = { .u64= { 0x0e8a35b10828d569, 0x5c3747465cc36c27, 0x3fdf57cd2c006497, 0x00000000ac3a5b26 } };
    const union Fixed256 expect = { .u64= { 0xd34519a797657777, 0x6c8f4922b68860ac, 0x178a8238a6055765, 0x000000000d9d74f9 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa0b4a2969d8055a9, 0xbf67da14be11d56b, 0x49390aa51cf5192b, 0x000000005653a45c } };
    const union Fixed256 b = { .u64= { 0x4ef25c0707e33868, 0x8d2f527e72daf0a5, 0x9aeb7fa0c4169b14, 0x00000000bd6d2500 } };
    const union Fixed256 expect = { .u64= { 0x51c2468f959d1d41, 0x323887964b36e4c6, 0xae4d8b0458de7e17, 0xffffffff98e67f5b } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x4391b6e2e6eacb0f, 0x66dfe717c1731339, 0x9f20dbb0dcc93f0e, 0x00000000b4917fc0 } };
    const union Fixed256 b = { .u64= { 0xb310b730049dd332, 0xf58105748ed5d1b7, 0xaba9e7b823fc5ad2, 0x000000000e893302 } };
    const union Fixed256 expect = { .u64= { 0x9080ffb2e24cf7dd, 0x715ee1a3329d4181, 0xf376f3f8b8cce43b, 0x00000000a6084cbd } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x21b7379f0897246a, 0x2bb3b36f29421c40, 0x74115c86188b1044, 0x00000000a294523d } };
    const union Fixed256 b = { .u64= { 0xea990e94821d4606, 0xb5459d83fbc46f1a, 0x0809eae3ef232a32, 0x00000000fe9934ad } };
    const union Fixed256 expect = { .u64= { 0x371e290a8679de64, 0x766e15eb2d7dad25, 0x6c0771a22967e611, 0xffffffffa3fb1d90 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xb6c004cc3b8367dc, 0x12d4050771d7b14e, 0x1497024640332b06, 0x00000000975b54a3 } };
    const union Fixed256 b = { .u64= { 0x475b51096c4ad652, 0xc02c6b9586b4625b, 0x26af8090013c3273, 0x0000000009167d41 } };
    const union Fixed256 expect = { .u64= { 0x6f64b3c2cf38918a, 0x52a79971eb234ef3, 0xede781b63ef6f892, 0x000000008e44d761 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x290535d868a24b7f, 0x8317cba01c75f67e, 0x167b75dfb948f82a, 0x000000003da95cd2 } };
    const union Fixed256 b = { .u64= { 0x0511baeb198ababb, 0xc02659fe2e87d415, 0x1aeffafc3b45402a, 0x0000000037a47ce4 } };
    const union Fixed256 expect = { .u64= { 0x23f37aed4f1790c4, 0xc2f171a1edee2269, 0xfb8b7ae37e03b7ff, 0x000000000604dfed } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xab63ad02854efa60, 0x7430051376e31f5a, 0x8918b6824f4a353e, 0x00000000a44a4d46 } };
    const union Fixed256 b = { .u64= { 0x7ad45a77cd7acfcb, 0x04fcd49f5dbe3b83, 0xf5ee773384eaed1f, 0x00000000ebe42b82 } };
    const union Fixed256 expect = { .u64= { 0x308f528ab7d42a95, 0x6f3330741924e3d7, 0x932a3f4eca5f481f, 0xffffffffb86621c3 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x5dd1d1839c4a67c3, 0xb0bdb4fe4a21f703, 0xeeb4de7afbc80976, 0x000000005f4a3ff5 } };
    const union Fixed256 b = { .u64= { 0xdfd5349d04e0cb95, 0x69897f7faf708536, 0x1ae25cc819e5bea1, 0x000000004e50ed89 } };
    const union Fixed256 expect = { .u64= { 0x7dfc9ce697699c2e, 0x4734357e9ab171cc, 0xd3d281b2e1e24ad5, 0x0000000010f9526c } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xc67c87efd73253cf, 0xd34d0494ac2da99d, 0xcfe330cd04065c81, 0x0000000073907cfc } };
    const union Fixed256 b = { .u64= { 0x381b37241398aa12, 0x7d8dd474c146a389, 0x1da2fee7313e72a8, 0x00000000925147db } };
    const union Fixed256 expect = { .u64= { 0x8e6150cbc399a9bd, 0x55bf301feae70614, 0xb24031e5d2c7e9d9, 0xffffffffe13f3521 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x4110c7681f29a9d5, 0x1f7a35fc1f2c5349, 0x9dd5a943149c59af, 0x00000000d99e3ea3 } };
    const union Fixed256 b = { .u64= { 0x786a7648a8beb003, 0x0b0ddfd1c6f75c81, 0xb4af8aa6b9387e62, 0x000000007f799eaf } };
    const union Fixed256 expect = { .u64= { 0xc8a6511f766af9d2, 0x146c562a5834f6c7, 0xe9261e9c5b63db4d, 0x000000005a249ff3 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xf5a8513f5b8aaa46, 0x243725d175004aff, 0x5ffa441acc790cf4, 0x0000000044d96a45 } };
    const union Fixed256 b = { .u64= { 0xdda4a33d86bbd79d, 0xb83438617a415782, 0xba588a82f4670327, 0x00000000cde9e144 } };
    const union Fixed256 expect = { .u64= { 0x1803ae01d4ced2a9, 0x6c02ed6ffabef37d, 0xa5a1b997d81209cc, 0xffffffff76ef8900 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x7dfbad9cee9b14f6, 0xae2321e6d6047617, 0x650936624bf8b43a, 0x000000003b4bee51 } };
    const union Fixed256 b = { .u64= { 0x1595661deb2812ef, 0xba51289f95fd948c, 0x93646be0d15ed4a4, 0x00000000189000c9 } };
    const union Fixed256 expect = { .u64= { 0x6866477f03730207, 0xf3d1f9474006e18b, 0xd1a4ca817a99df95, 0x0000000022bbed87 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xe2999fc1abb51d18, 0x545484cfae5c812f, 0x2c2af60d70583376, 0x000000008624857a } };
    const union Fixed256 b = { .u64= { 0x27f0ce161cb0a312, 0xfc9779298a77fac2, 0xf807214ef32f2f10, 0x00000000c17a9f18 } };
    const union Fixed256 expect = { .u64= { 0xbaa8d1ab8f047a06, 0x57bd0ba623e4866d, 0x3423d4be7d290465, 0xffffffffc4a9e661 } };
    Fixed256_sub(&got, &a, &b);
    CHECK(got, expect);
  }
}

static void fixed256_test_mul(void) {
  union Fixed256 got;
  {
    const union Fixed256 a = { .u64= { 0x97b750923ceb3ffd, 0x216363698b529b4a, 0xea7b5bf55eb561a4, 0x000000009a9a80fd } };
    const union Fixed256 b = { .u64= { 0x94b2b8fda02f34a6, 0x9b08923d10c67fd9, 0xe8a8529f035efa25, 0x00000000d6645fa9 } };
    const union Fixed256 expect = { .u64= { 0x96ae463df3a72172, 0x7d5a61a50d19a051, 0xe406ac04f8d59b39, 0x8179c5f49791c4e2 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x8cb4a0d7d6225675, 0x65aa9c8279f248b0, 0xdc6bf1e1a399f82a, 0x00000000268ecc45 } };
    const union Fixed256 b = { .u64= { 0x26d0b944a2863a7f, 0xed038db4de383784, 0x63d2e49085ef3430, 0x00000000bdc2ae99 } };
    const union Fixed256 expect = { .u64= { 0x4d125bf111c85f95, 0x0b4413afa620b325, 0x85ab79f2f917fea0, 0x1c94bf5289e54982 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xc6f8da3eabe19f58, 0x28ce6f2410645d51, 0xf51e8722c21b6092, 0x0000000097524d6a } };
    const union Fixed256 b = { .u64= { 0xeb8f624fb804d820, 0x633a50eee0f9e038, 0xc9c18070b6d13089, 0x00000000ebcd1f5e } };
    const union Fixed256 expect = { .u64= { 0xfc4aa03eecf7113d, 0x0571d60cf05fef8d, 0xf176ad3ba6944649, 0x8b61cc842c83b034 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xba6676b3651c5253, 0x93b05a04cd085b71, 0xf6ced90a71d2af72, 0x00000000ef829c88 } };
    const union Fixed256 b = { .u64= { 0x5d92b243e0fd67dd, 0x092fdddf18f2c41c, 0x7eb0adf422cedafb, 0x00000000378c74dc } };
    const union Fixed256 expect = { .u64= { 0x2cdf54cd02b2816b, 0x2c14446a288acec3, 0x78bfe29877ebc90a, 0x33f8786481a194e0 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xac0ae4e2f729b4c8, 0xc76abf436fa84dca, 0xdaf0105ba06c05a1, 0x000000004d100d8f } };
    const union Fixed256 b = { .u64= { 0xd55ec1a581daad10, 0x92f3277b62c82185, 0x88bafad959d54505, 0x0000000095c76ab4 } };
    const union Fixed256 expect = { .u64= { 0x3c170cdbcc2ddb90, 0xcd3e1e29733012ea, 0xc4e361cdf2626488, 0x2d165f7e4350d450 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x3b7dae0495918694, 0x56363b4be779c470, 0xea6d5547ae966193, 0x00000000edcf6109 } };
    const union Fixed256 b = { .u64= { 0x538e504edc52bdca, 0x8ab12c32f6f22f41, 0x926baeafe79a27e6, 0x0000000091b1078e } };
    const union Fixed256 expect = { .u64= { 0x5f5b5e4c8b2ab9d2, 0x58d9fb2c9823e5a8, 0x2921e49b0973fa6e, 0x8756e95dd256691a } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa7cf94d7b6bcb64f, 0xa20ab57c360c4979, 0xfcf249f3d4e441c3, 0x0000000092d3043a } };
    const union Fixed256 b = { .u64= { 0x1fdaf62548f2f8ed, 0x7b6471e2103ef3c2, 0xa385ac4bda9bf98c, 0x00000000fd63ed5b } };
    const union Fixed256 expect = { .u64= { 0x17788eaf783636a0, 0x1d8b6d8ec4e546f2, 0xd506a3b6f11e53ba, 0x9153daead8816810 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x5815a3d516a91f39, 0x110d7c25ccf3d0b3, 0xe5a8181b691406be, 0x0000000026988f4f } };
    const union Fixed256 b = { .u64= { 0xec007b1be1830294, 0x47715c45fb0af1e3, 0x3c67523f81633acf, 0x00000000ff9e4840 } };
    const union Fixed256 expect = { .u64= { 0xfd1b55cd7a855c2d, 0x93b09438f180c73a, 0xb35108115e29af20, 0x2689d3cda09fcfb1 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xfad9d3a90add12e3, 0x56fe09f7de26c45b, 0x5c35d7ed5057326c, 0x00000000f56ab44e } };
    const union Fixed256 b = { .u64= { 0xdc98da8ae58b7c6a, 0x6072c48f60b6cbb1, 0xdeb135fa75dd67de, 0x00000000852380c4 } };
    const union Fixed256 expect = { .u64= { 0xdc2e71cfd2c9ef73, 0x66b85ac5dbd824b7, 0x260a082b2857f28a, 0x7fa278b571f43986 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xefba436b3cd5b001, 0x6ffc71e44d14075d, 0x421b8cb9fa50ecd7, 0x0000000085689935 } };
    const union Fixed256 b = { .u64= { 0x56c2adc08c65f067, 0xc9d459c502eee0ab, 0xfcc9e97f6a4b3989, 0x00000000947899a4 } };
    const union Fixed256 expect = { .u64= { 0xdec07a46801bea78, 0x52f7c6ecd4407aff, 0x9e43706ac28a9d25, 0x4d5f51ac197e1485 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x0570ceeead0faada, 0x5e819615f69b31ce, 0xa0c2995f40498cb3, 0x0000000074d0df35 } };
    const union Fixed256 b = { .u64= { 0xd805f5d25e80dfff, 0x439e7fa9987aa6bd, 0xc98c9e514ce74654, 0x00000000608d9499 } };
    const union Fixed256 expect = { .u64= { 0xe007d6628c2a13c4, 0x4064612af488ff88, 0xf608b13a3adf6245, 0x2c0eee8dfaab1668 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xd0247e4cc5b3b5d3, 0xf8abffd606e44edf, 0xaf091db491bae46a, 0x00000000bc344f4b } };
    const union Fixed256 b = { .u64= { 0x8000b3d94f5d410c, 0xa75a68a138f83d74, 0x44f5f725cdc656fb, 0x000000003d1b2085 } };
    const union Fixed256 expect = { .u64= { 0xfbffb1573e09c68d, 0x5a70e4d5db89aa43, 0x7d7e0781a85851e2, 0x2cec6851e97a030f } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xf5efd434db045aae, 0x14777e962b56363c, 0xbdf84ab55632a446, 0x00000000a675a109 } };
    const union Fixed256 b = { .u64= { 0x9182c3c8e288b164, 0x454608a5737b6ed7, 0xc9794969399b6cad, 0x000000001ef491a6 } };
    const union Fixed256 expect = { .u64= { 0x008f1206b554befe, 0x524ced38a0faa3b6, 0x00a410d9bc471085, 0x1420cfb1b50a5319 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xdd1d40962eff832f, 0x57116d4c4751d092, 0xd3f44c52cea663ee, 0x00000000a4560002 } };
    const union Fixed256 b = { .u64= { 0xd997c6f7cb3a88f6, 0x76f7f138456bb11b, 0xa25994fc58aaac81, 0x000000006aba54ef } };
    const union Fixed256 expect = { .u64= { 0x12c1cdade0bd86ad, 0xabd3d3b24fc5b066, 0x488a84c97ff78e31, 0x448339033a5404d5 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x917e39166b761fc5, 0x09196da468d6710e, 0x69cbc6d1ebad40d0, 0x0000000027ef79cb } };
    const union Fixed256 b = { .u64= { 0x8f1233c76f31b692, 0xfc5ab8f2f33dc30a, 0xb79e4444ed6897d8, 0x0000000038daf051 } };
    const union Fixed256 expect = { .u64= { 0x31c65240a8140948, 0x091b10b2306289b5, 0xaf5aaad6978afe6e, 0x08de8a0f77f5c814 } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xf8911b0496b3952d, 0x1eb8143249799084, 0x3e99c6c8cf68bc28, 0x000000000b8a276b } };
    const union Fixed256 b = { .u64= { 0x6e0d0eb1e651171d, 0x0ca2a6b393b337fb, 0x7b25f34a035d7017, 0x00000000becbde01 } };
    const union Fixed256 expect = { .u64= { 0x866a72c69c893a09, 0x0cdc27a3d1612183, 0xf73d1334f2edb139, 0x0899b9d0b9c7040d } };
    Fixed256_mul(&got, &a, &b);
    CHECK(got, expect);
  }
}

static void fixed256_test_div(void) {
  union Fixed256 got;
  {
    const union Fixed256 a = { .u64= { 0x4da4f9fc3c6da5d7, 0xb8a1abcd1a6916c7, 0x7a97c643656412a9, 0x0000000027ac435a } };
    const union Fixed256 b = { .u64= { 0xd3addccb2c33be0a, 0x43000de01b2ed40e, 0xf165c8ce36e2f24b, 0x00000000ed6f0b09 } };
    const union Fixed256 expect = { .u64= { 0xb8470ab10a829297, 0xa86744cbb25624e6, 0x2ac66fc354059f19, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa4042bb3d4341aad, 0x42a00403ce80c4b0, 0x459142deccea2645, 0x000000003184ff27 } };
    const union Fixed256 b = { .u64= { 0x5f552773e14b0190, 0xd8441b5616332aca, 0x566002249b191bf4, 0x00000000abf4a07c } };
    const union Fixed256 expect = { .u64= { 0x56671e3c416d0ef7, 0x19648c9f41a8e266, 0x49b8f67c27fd9023, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xd160c5d0ef412ed6, 0xf1347e0cdd905ecf, 0xd7288ff68c320f89, 0x000000004cdce7a6 } };
    const union Fixed256 b = { .u64= { 0x31f3b9238224b122, 0x6c79a3de69f85e31, 0x49c7b59b995253fd, 0x000000006e58d5ca } };
    const union Fixed256 expect = { .u64= { 0xab694824c7928bf0, 0xbf63e1ff015439c3, 0xb25183fd11c46781, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x3bb4a570294c4ea3, 0x4278c2614e1bcb38, 0xcc21ce88d0060cc5, 0x000000000b11ad28 } };
    const union Fixed256 b = { .u64= { 0xb36cc9aa78a330a1, 0x2522d53857c49391, 0xac7cc4a4ff4dab10, 0x0000000032111ac1 } };
    const union Fixed256 expect = { .u64= { 0x273bf24193b5f904, 0xcde5ee671a9c63bf, 0x38990fc489fcc97c, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xe9dd38b869ace913, 0xa2909cb633e238b4, 0x70ef55b1a1f65507, 0x0000000046bbe9e8 } };
    const union Fixed256 b = { .u64= { 0x6f98bca35b17b966, 0x96b98b5fbf37a2be, 0xa26a25c852175b7a, 0x000000008efbc170 } };
    const union Fixed256 expect = { .u64= { 0xe4df3be9c75c0f10, 0xeec87a87a1bbf8cc, 0x7ea4b929721a1012, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x52d32377e78131c1, 0xd6e4a51519d9c9cc, 0xb54a23020fc5b043, 0x000000003a973000 } };
    const union Fixed256 b = { .u64= { 0x4a7a03052d733dcd, 0x0692b534758240df, 0x5b69dc230af5ac87, 0x00000000b281b888 } };
    const union Fixed256 expect = { .u64= { 0x2c713e3dfa2b60cc, 0xefadf16ca983d8d0, 0x5406a13694b13bc1, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xf6e7d078e55b85dd, 0x4922b9ccf469aef8, 0xacdac615bc20f626, 0x00000000f5b9e1f5 } };
    const union Fixed256 b = { .u64= { 0xdc82f2526911c9dd, 0xde97faf0f17ca82c, 0xae17584a9ed9c621, 0x00000000d163b764 } };
    const union Fixed256 expect = { .u64= { 0x6c0186b4cee93454, 0x6f2a2609d19d1f20, 0x2c6cd85f9724dc76, 0x0000000000000001 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x9e2387a54b1cef39, 0xe4e2aafd31009624, 0x4ac034cf71b34e47, 0x0000000022ed9387 } };
    const union Fixed256 b = { .u64= { 0x994b971761b2ceba, 0x28adf9c6f6396ae3, 0x92b607d554d08ce6, 0x000000000267ce8c } };
    const union Fixed256 expect = { .u64= { 0x71d0e8997aceacb0, 0x0aaa15af3d41fa35, 0x85253847f32dfd00, 0x000000000000000e } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x52496e1e3fc24ec0, 0x091b5ffbff651b90, 0xd5b8c2551f4d4cc5, 0x00000000877c606f } };
    const union Fixed256 b = { .u64= { 0xff353728c6173d94, 0xa6d00e3468c946b0, 0x33445533fcd71d42, 0x000000007a4e9ba3 } };
    const union Fixed256 expect = { .u64= { 0x26a5562a005b4e67, 0xd23f31cd3cea6d2c, 0x1b959c36487fa967, 0x0000000000000001 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x705620733deaaddd, 0x7defb12b691e8e3b, 0x38146a2f0970425b, 0x000000006bd5231f } };
    const union Fixed256 b = { .u64= { 0xa5be57d93fa3549b, 0x6d86b88de3a9312c, 0x37396957d4bf8115, 0x000000007fa2ebbc } };
    const union Fixed256 expect = { .u64= { 0x10c04cae5d46583f, 0x7249f9239083fefd, 0xd8478c783625d3f5, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa728e00ee6a4ccec, 0xdef32daea76ace09, 0xb7a46957ca75a6c1, 0x00000000bea661c3 } };
    const union Fixed256 b = { .u64= { 0xf1b31705e656cae2, 0x2a5b4beaecb11a5a, 0x4bd411e6562abc30, 0x00000000a82b302f } };
    const union Fixed256 expect = { .u64= { 0x620ed7263ca27484, 0x766eb88b78538c2e, 0x2239056cf21ec598, 0x0000000000000001 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xe6941cdfccac5657, 0x50a64652a47a7b5e, 0x6b8468c8d09872a7, 0x00000000872c92ea } };
    const union Fixed256 b = { .u64= { 0xcd285f3ba79c875d, 0xcd152d1aaf9b1084, 0x56a9f13444af3f13, 0x00000000ec3c9e45 } };
    const union Fixed256 expect = { .u64= { 0xcc7f7856db85a039, 0x3a4eba678a47f703, 0x927b8a3203ec130b, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x76ee7876a29bcc45, 0x78e100a991b77af5, 0x675a9879bf1a4478, 0x00000000ed6f642c } };
    const union Fixed256 b = { .u64= { 0x41e0f8f2e05d4bd0, 0x65520b8f1daaf702, 0xc50f9b0fcf53cb25, 0x00000000c870b446 } };
    const union Fixed256 expect = { .u64= { 0xcbb54bcf0a703ea3, 0x284b356cee843a46, 0x2f3fda7afec2ef35, 0x0000000000000001 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x38eaf8cae0bc9aa3, 0x0dabacd08cf4ac18, 0xf37207e3e0f2f8e9, 0x0000000033a09bf9 } };
    const union Fixed256 b = { .u64= { 0x742a41bb8b1add60, 0x3491fea161939295, 0xfa0052fec99239cc, 0x000000004fa8cbd5 } };
    const union Fixed256 expect = { .u64= { 0x7b010b18af49b2f1, 0xb90615ebe22445f1, 0xa5ea00fcf9450247, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xc23aed933bd3cd5a, 0xdb0063007c9539a1, 0x666a32ecf75e08f2, 0x000000001859c35c } };
    const union Fixed256 b = { .u64= { 0xf0d2c0c5e19e82c4, 0xa30e4c9332236680, 0xee0413f84d63e406, 0x00000000d6210fa3 } };
    const union Fixed256 expect = { .u64= { 0x3a71371eadd445b9, 0xe49eff9309c1bf0e, 0x1d1cb6e2b132cda5, 0x0000000000000000 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x1e408d4e6c26a143, 0x8eec2b7acc609760, 0x5f215ed42ad93787, 0x00000000dfd14872 } };
    const union Fixed256 b = { .u64= { 0xb4ebf26a2d2555df, 0x5399187b2638fb22, 0xdc1d41417e8782b3, 0x0000000056ad802d } };
    const union Fixed256 expect = { .u64= { 0x2d800346864066d1, 0xc952e33131896f35, 0x9509be286a210cce, 0x0000000000000002 } };
    Fixed256_div(&got, &a, &b);
    CHECK(got, expect);
  }
}

static void fixed256_test_compare(void) {
  bool got;
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000 } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000 } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000 } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000 } };
    const union Fixed256 b = { .u64= { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001 } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001 } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001 } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001 } };
    const union Fixed256 b = { .u64= { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    const union Fixed256 b = { .u64= { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff } };
    const union Fixed256 b = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff } };
    const union Fixed256 b = { .u64= { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0x4da4f9fc3c6da5d7, 0xb8a1abcd1a6916c7, 0x7a97c643656412a9, 0x1710cf5327ac435a } };
    const union Fixed256 b = { .u64= { 0x4da4f9fc3c6da5d7, 0xb8a1abcd1a6916c7, 0x7a97c643656412a9, 0x1710cf5327ac435a } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0x66ceab360512bd13, 0xeaff1a098ca59966, 0xccea71ff4a14876a, 0xc3e1b258fd724452 } };
    const union Fixed256 b = { .u64= { 0x66ceab360512bd13, 0xeaff1a098ca59966, 0xccea71ff4a14876a, 0xc3e1b258fd724452 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0x8534f45738d048ec, 0x5c3902b38963dc6e, 0xc79d679346d4ac7a, 0xd3addccb2c33be0a } };
    const union Fixed256 b = { .u64= { 0x36e2f24b43000de0, 0xed6f0b09f165c8ce, 0xd4341aad06905269, 0xce80c4b0a4042bb3 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xdaca3c06f5ff0c03, 0xe14b0190d93936e1, 0x16332aca5f552773, 0x9b191bf4d8441b56 } };
    const union Fixed256 b = { .u64= { 0x634f806fabf4a07c, 0x3fb62d2c81862fc9, 0x3f5082492d83a823, 0x47adec26793d0e45 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xef412ed6f1cfd992, 0xdd905ecfd160c5d0, 0x8c320f89f1347e0c, 0x4cdce7a6d7288ff6 } };
    const union Fixed256 b = { .u64= { 0x69f85e3131f3b923, 0x995253fd6c79a3de, 0x6e58d5ca49c7b59b, 0x294c4ea3738d243a } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x47ca7883ff5a52f1, 0x88dcf94384d4cd1f, 0x78a330a1a5e333cb, 0x57c49391b36cc9aa } };
    const union Fixed256 b = { .u64= { 0x32decd6b8efbc170, 0x52d32377e78131c1, 0xd6e4a51519d9c9cc, 0xb54a23020fc5b043 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xf469aef8f6e7d078, 0xbc20f6264922b9cc, 0xf5b9e1f5acdac615, 0x04a7f00753be4721 } };
    const union Fixed256 b = { .u64= { 0x4ac034cf71b34e47, 0x40031ad622ed9387, 0x994b971761b2ceba, 0x28adf9c6f6396ae3 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xff4788955cdb7f4c, 0x924aecbe4a53583b, 0x7077b81d18dbb0c1, 0x350d775dfb53e13d } };
    const union Fixed256 b = { .u64= { 0xf87a7976ad448abd, 0x9b44baf5264ed787, 0x8bd5bb710a77ec0c, 0x952989c17d9c649a } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xa6d00e3468c946b0, 0x33445533fcd71d42, 0x33a760e17a4e9ba3, 0x705620733deaaddd } };
    const union Fixed256 b = { .u64= { 0x0970425b7defb12b, 0x6bd5231f38146a2f, 0x3fa3549b71895aa3, 0xe3a9312ca5be57d9 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x37396957d4bf8115, 0x300dc4c27fa2ebbc, 0x096c1dbb081a3cfe, 0x40ddfed8411ff179 } };
    const union Fixed256 b = { .u64= { 0x3543c7a68692c6f3, 0x3b416610c5b67999, 0xdea5486a6ac9573d, 0x2446913842fdef77 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xe41a3f3d0d204649, 0x50842aaaed939512, 0x1dea467190ba65d0, 0x91d8731efd960ad6 } };
    const union Fixed256 b = { .u64= { 0xe6a4ccecf6772033, 0xa76ace09a728e00e, 0xca75a6c1def32dae, 0xbea661c3b7a46957 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x85e26df5e10ad788, 0x02b37cf9e7b06b43, 0xd672c8b84c042869, 0xd3db11b758cf43fe } };
    const union Fixed256 b = { .u64= { 0xf5b9e8e715cbcd0b, 0x742a41bb8b1add60, 0x3491fea161939295, 0xfa0052fec99239cc } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x3bd3cd5a637d4bf4, 0x7c9539a1c23aed93, 0xf75e08f2db006300, 0x1859c35c666a32ec } };
    const union Fixed256 b = { .u64= { 0xea6ef21a1d4cab27, 0xcc60e6739ef7590e, 0x5daa78bce2a5d4ff, 0xffe4d7b9833189cb } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x2ad937878eec2b7a, 0xdfd148725f215ed4, 0x2d2555df29ae9cd7, 0x2638fb22b4ebf26a } };
    const union Fixed256 b = { .u64= { 0x5b56a0604b1f9ac5, 0xc4447b043aad7091, 0xfc5db3502e2f91d5, 0xa06f72b1dd2bc5f7 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0d8c202bad3e65b8, 0xe85ae7849c0a7a5d, 0x8b4923be5032a185, 0xe8be2948e9aca3d3 } };
    const union Fixed256 b = { .u64= { 0xe9520b19c84ed939, 0x4e3f1ad0917eabe5, 0xde16309eefcdfc39, 0xc96ef60381b48a3a } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x9e11d1449ced8925, 0x644d686a712b3440, 0x406f29f62402ca31, 0x98dac428c5630494 } };
    const union Fixed256 b = { .u64= { 0xe77f3fbfefaa0591, 0x5463ca582dff38da, 0x5f346b34a77d1908, 0x2cb4145953a0cf68 } };
    got = Fixed256_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
}

static void fixed256_test_compare_unsigned(void) {
  bool got;
  {
    const union Fixed256 a = { .u64= { 0x05ee8f05e21eeb01, 0x052545f09a90a0b9, 0xf30b5d73862097a9, 0xd46c10dac12980f3 } };
    const union Fixed256 b = { .u64= { 0x05ee8f05e21eeb01, 0x052545f09a90a0b9, 0xf30b5d73862097a9, 0xd46c10dac12980f3 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xe18bbbbb67f6ead6, 0x3cc873db995a4ce9, 0x7d65dfc7e59993c4, 0x389b497cb41e70d1 } };
    const union Fixed256 b = { .u64= { 0xe18bbbbb67f6ead6, 0x3cc873db995a4ce9, 0x7d65dfc7e59993c4, 0x389b497cb41e70d1 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x3af96fde5e1c2969, 0x539d339fbee46e99, 0xa075c01d8987af02, 0xff174aee879e66f0 } };
    const union Fixed256 b = { .u64= { 0xcd879bb1e62607b8, 0x819be1a066749dbc, 0xf831ff826691a7ec, 0x50290c45d1471c0e } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x695c49807033d01d, 0x035ffb53968ffab6, 0x2f1390eb40541d28, 0x8a4a672de5babcb8 } };
    const union Fixed256 b = { .u64= { 0x58ef78a0eb82c4f1, 0xcbd6504b9a92dfa9, 0xe90669df4feec135, 0xc16ca0e317620383 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0x38df94187bc3a303, 0xeda16b2fa3d307fc, 0xe47f10467a12860e, 0xc725b20c9a8768a2 } };
    const union Fixed256 b = { .u64= { 0xec8b43822612f9b5, 0x11af27103d047879, 0x21f4dea74c9942fe, 0x0c009fddf2ab6e3f } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x04be342243287054, 0x3ed198e9863cebef, 0xc055bbbe28533065, 0x349ae3a218c56efa } };
    const union Fixed256 b = { .u64= { 0xf3de70f26a3ace5d, 0xe0e4206c2147494e, 0xb82cca76b58c1f17, 0x8f59675414d09501 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0x00aa4578b19a6188, 0xe16b31fdb944d477, 0x27e14aff8d6710a7, 0x6973a73e80a71048 } };
    const union Fixed256 b = { .u64= { 0xd826608b370de6ed, 0x7bf3fdd24c517ded, 0x115b6b4781f1a426, 0x2b04527a603f852a } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x54137809668f0a56, 0x6305b03b2b96664d, 0x6c4f61950fda0caa, 0x072e60ebfaff16a9 } };
    const union Fixed256 b = { .u64= { 0xad1c476fe06c431d, 0x4dd2c99504dd27b6, 0xca1a5205f5c8bf79, 0x15f848fc270b1920 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0xd93d1e6b1daff6d6, 0xd2ca2f479bfdd7a0, 0x3b658bd203efb5b5, 0x8db3d5a93b34e5e8 } };
    const union Fixed256 b = { .u64= { 0xb4da712abb508a6e, 0xebd5b306de9b7ed9, 0xefe751e9aefe694d, 0x61242e10b2860229 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xe9679cd3e7303dd8, 0x4e1e30495b565cc2, 0xa597dd1bee32a308, 0xe5ac6e61d4f36f94 } };
    const union Fixed256 b = { .u64= { 0xa888d4cc1a82d1c3, 0xb2dff7a7f9bf5bd6, 0x9acc664d09ca2aa8, 0x86ef837852857138 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0xf48743d5fcc920ce, 0xeda5697d5a6219a1, 0x2d58c1ee14dde0b7, 0x0b0c4895b6a893b7 } };
    const union Fixed256 b = { .u64= { 0x561dc2b21243a0d2, 0x9a0a2dfcc8161be5, 0x2348816ee890f855, 0x4efd5a78b25b9d01 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0xfa80137481ef1c07, 0xd3a1a0e7e3855179, 0x826188ead32bd235, 0x003b13d5d6c16f48 } };
    const union Fixed256 b = { .u64= { 0x08c7ac49f9335775, 0x7a638e1d02b08fb3, 0x2515c174b29b3519, 0x5c1e1f6236907e45 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0xbd09713ee3e1ae85, 0x584f878fbf969bfc, 0xfb8930dbd0dff051, 0xdb9fbff04c520178 } };
    const union Fixed256 b = { .u64= { 0x9ad492ffa1d735a1, 0x08b6f79467129533, 0x698c49232c40e270, 0xf99231987e62c170 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0xb45e50b035242ef1, 0x6ff994fcb4108c78, 0x2cc43ab052646e87, 0x0498119036f72908 } };
    const union Fixed256 b = { .u64= { 0x6fed5666e312dffb, 0xf8c42f8dee8c9bb3, 0x54d37a3d46a4565f, 0xe8961d66c17b052e } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const union Fixed256 a = { .u64= { 0xa0a52b2df32ce83c, 0xd7874408ed7d0b4c, 0x523aabc668d90aa1, 0x9ebfefa0c01288b6 } };
    const union Fixed256 b = { .u64= { 0x4796a2f14188715e, 0x162c769a82d8c192, 0x672d879b07b37f1e, 0x9be0fd5543796067 } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const union Fixed256 a = { .u64= { 0x0209526b572ad3d6, 0x91f07de174a3e170, 0x3427f7e1fd84d835, 0x17f06baa2a9db829 } };
    const union Fixed256 b = { .u64= { 0xad35baa62e5b4755, 0x434f3e0e34c6c870, 0x97fa613b08ec43e2, 0xa1a1afc4530d03ee } };
    got = Fixed256_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed256_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed256_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
}

static void fixed256_test_2str(void) {
  char buffer[1024];
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "0");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "0");
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "1");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "1");
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xffffffffffffffff } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "-1");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "-1");
  }
  {
    const union Fixed256 a = { .u64= { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0x7fffffffffffffff } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "9223372036854775807.999999999999999999999999999999999999999999999999999999999840690808886754772297111960223228819440889544480738121392611414661383709848694183905691012527981731405901655307388864457607269287109375");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "7fffffffffffffff.ffffffffffffffffffffffffffffffffffffffffffffffff");
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8000000000000000 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "-9223372036854775808");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "-8000000000000000");
  }
  {
    const union Fixed256 a = { .u64= { 0x91b7584a2265b1f5, 0xcd613e30d8f16adf, 0x1027c4d1c386bbc4, 0x00000000414c343c } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "1095513148.063106824123248136514924942601119395451371075807577212323547280771304176783038720530665589676312382931924069699814527303037321485577281951819658128576169542345297713836771436035633087158203125");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "414c343c.1027c4d1c386bbc4cd613e30d8f16adf91b7584a2265b1f5");
  }
  {
    const union Fixed256 a = { .u64= { 0xc2ce6f447ed4d57b, 0x78e510617311d8a3, 0x612e7696a6cecc1b, 0x00000000c9e9c616 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "3387541014.379615222721265856244319519334130246337026364505782495889985538533727834896106265248552833617858507031815056973450076427515785298891617384261949402833417888114553306877496652305126190185546875");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "c9e9c616.612e7696a6cecc1b78e510617311d8a3c2ce6f447ed4d57b");
  }
  {
    const union Fixed256 a = { .u64= { 0x9755d4c13a902931, 0x1a2b8f1ff1fd42a2, 0x51431193e6c3f339, 0x0000000007d4bedc } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "131383004.317429636582133234732555277501731532457887484725350323376644412974175942374944425119870641458192391220514079513460882843192680326024223547247481323958263887963227034560986794531345367431640625");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "7d4bedc.51431193e6c3f3391a2b8f1ff1fd42a29755d4c13a902931");
  }
  {
    const union Fixed256 a = { .u64= { 0xc2cd789a380208a9, 0xf3c64af775a89294, 0xed2f89d94a2f20aa, 0x0000000005805975 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "92297589.926506629513991440235984377532305833232778585891624713081392969349025185036798382610909487770872549480485359913610772872722940951378798394275405439523586592631687608445645309984683990478515625");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "5805975.ed2f89d94a2f20aaf3c64af775a89294c2cd789a380208a9");
  }
  {
    const union Fixed256 a = { .u64= { 0x1622bd795fec898f, 0xa9ec0806705fca16, 0x1ba1621582283d15, 0x00000000c74803e3 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "3343385571.107931261302071272509595838365742999449560966714946829493525409409169267966322978927048133682094682700433893850967532208650338028320277406414510684511641469551790351033560000360012054443359375");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "c74803e3.1ba1621582283d15a9ec0806705fca161622bd795fec898f");
  }
  {
    const union Fixed256 a = { .u64= { 0xd707107e855c3844, 0x5eda92d864ac5db9, 0xbb968a437d5c8dfc, 0x0000000007923986 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "127023494.7327658095140227865446951618161115453781794871737372043155151304625869590487432059012659430632062749362696772897648650771527916149893975067476748299667710373483942021266557276248931884765625");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "7923986.bb968a437d5c8dfc5eda92d864ac5db9d707107e855c3844");
  }
  {
    const union Fixed256 a = { .u64= { 0xa5ac06d864c2f2e3, 0x2b28fef02b9c014e, 0x3a1890c78092b4d4, 0x00000000fb695ffb } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "4217987067.226937340456582270897583790038249726796749256400927902516298743104194674448984530680188407466355350820403664058092497068058681904692889057948439036449354844204862047263304702937602996826171875");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "fb695ffb.3a1890c78092b4d42b28fef02b9c014ea5ac06d864c2f2e3");
  }
  {
    const union Fixed256 a = { .u64= { 0x33138131c541013d, 0xeb8ac8ce8a245e6b, 0x8c5fe8f8dc3bf364, 0x000000003b6fe507 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "997188871.548338471180826839055087557404126775316215713422931589584344897182944208415814842593176208560944565798940946266752399361333007156403892140825134698706340430351957593302358873188495635986328125");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "3b6fe507.8c5fe8f8dc3bf364eb8ac8ce8a245e6b33138131c541013d");
  }
  {
    const union Fixed256 a = { .u64= { 0xa8c24d4244ef7feb, 0x9be3cecb8c497c68, 0xbab9f87ff5059285, 0x0000000001762741 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "24520513.729400187720804539819179644723061852844016969198231643290260383739874714679864652843582337350059122026203357202194477134930230153359597085397517934597481914504868427684414200484752655029296875");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "1762741.bab9f87ff50592859be3cecb8c497c68a8c24d4244ef7feb");
  }
  {
    const union Fixed256 a = { .u64= { 0x69d495dd81355c53, 0xd037cdff7c240d49, 0x6a17b9af5b569643, 0x0000000058989008 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "1486393352.414424519836155223274863695125208325150613005620000770426734546387618322606055300630949390828844802264813909562520756175761477577563304735994068373513936343055519273548270575702190399169921875");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "58989008.6a17b9af5b569643d037cdff7c240d4969d495dd81355c53");
  }
  {
    const union Fixed256 a = { .u64= { 0xcdf8440407295e42, 0xa2a7ae1f3ac7652c, 0x8cfe5cd12d5db79b, 0x00000000959f3a51 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "2510240337.55075626475205907162034186165169355909465710553109092685412423025289852689776216936875159558384258304469951644621558085144664465099038725198992070522692465761593894058023579418659210205078125");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "959f3a51.8cfe5cd12d5db79ba2a7ae1f3ac7652ccdf8440407295e42");
  }
  {
    const union Fixed256 a = { .u64= { 0x73f7ba8e0445d656, 0xc10faa4003ba33db, 0x47fc816ac16e2284, 0x000000003fe31d03 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "1071848707.28119667869889800768905097070000175184848601192760892709670811395211302620463878222004302828072679755545827471415478043418229129152513013718123124017565783727690131854615174233913421630859375");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "3fe31d03.47fc816ac16e2284c10faa4003ba33db73f7ba8e0445d656");
  }
  {
    const union Fixed256 a = { .u64= { 0xcc1b0c3e1c07724e, 0x2f429ce59ff3078f, 0x4a5012dc582c18c9, 0x0000000011cbc288 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "298566280.29028432731153975801360022539082154477696855040758637210108674983198658474570186785003949163897405370246670862947815140263499720553823509592903694197067732130079775743070058524608612060546875");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "11cbc288.4a5012dc582c18c92f429ce59ff3078fcc1b0c3e1c07724e");
  }
  {
    const union Fixed256 a = { .u64= { 0x4b63e0efb62ac1fe, 0xb3df44a47467537a, 0x7f1a355e526eb523, 0x0000000079490eab } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "2034831019.49649365952285047703195471152685742093750495020503228938535969715058981433067596806886789670835077758059724014421842201334278433397739692269156369544384765912781176666612736880779266357421875");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "79490eab.7f1a355e526eb523b3df44a47467537a4b63e0efb62ac1fe");
  }
  {
    const union Fixed256 a = { .u64= { 0x4fdf8e1a060cea63, 0x57e54acc62f5680c, 0xcbd3f5e06bc15385, 0x000000003023580c } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "807622668.796203009882224415084578821414462513341557933920837207234083749525459639919628295291372922155681821950468538928869654032503481371027238178907619646040676186782292234056512825191020965576171875");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "3023580c.cbd3f5e06bc1538557e54acc62f5680c4fdf8e1a060cea63");
  }
  {
    const union Fixed256 a = { .u64= { 0x6e80fa489b0bca16, 0xf9bddea5d12982e4, 0x39b21c95055455e8, 0x000000000492c4f5 } };
    Fixed256_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "76727541.22537401807979170906355231287551214890934613422385422662193473652730477172991434566501398156010507596887642935909918741390180171178103255115996423978662333009737039901665411889553070068359375");
    Fixed256_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "492c4f5.39b21c95055455e8f9bddea5d12982e46e80fa489b0bca16");
  }
}

static void fixed256_test_str2(void) {
  union Fixed256 got;
  {
    const union Fixed256 expect = { .u64= { 0xb1eedaffcc3d5506, 0x736a947a843fdda7, 0x861e02ec39235bc0, 0x00000000a6048457 } };
    Fixed256_str2(&got, "2785313879.52389543788116326938576907408375224913564116370246742911465440415852077788275796139041929838038241274769221526335938332466774663940433898525337444765891115050493453964008949697017669677734375", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0xacc66a576518093d, 0xcdaaac43936aa40c, 0xa8ea37f7523d2a54, 0x00000000a185cc8e } };
    Fixed256_str2(&got, "2709900430.659823892480052497534248954421594295650176020641688353616015678016584592419844019908901833874567773531772931373546476092125159190080646049589951812369865991048101250271429307758808135986328125", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0xbcc99ae80f0c8a89, 0x202cc8284c717095, 0x364e433ff7c882f4, 0x00000000e023033d } };
    Fixed256_str2(&got, "3760390973.212131693951763052785829822515648265329667682844659611565179070402411739568289393814745869717375725152644788999860888332155017157291041350329931022161179809959463682389468885958194732666015625", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x121b28004e6f5a94, 0x1391f9b9dbc799b0, 0xeacc110e4f73fd94, 0x00000000f07534fe } };
    Fixed256_str2(&got, "4034213118.9171763095796023554154788011364909326580468018878913383770048025932320577218230347976876649018584961154979708731536213020827783192983289286213447713720647858082202219520695507526397705078125", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x9f7a7dafb43adc4f, 0x0994940e82458cc8, 0x334de73d60c290d0, 0x0000000058d07674 } };
    Fixed256_str2(&got, "1490056820.200407459721427605623486415691307005286887072437604873823524105409408454600673827817364839458339536755601437482086018104212910353637616013598438989367321594325943578951410017907619476318359375", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x92c9357d34accd78, 0xe585552fac954ab5, 0x976699cc6ed5d1bf, 0x0000000031b1c27e } };
    Fixed256_str2(&got, "833733246.591409313593280996224021416704959662587274164776887280221648907802756103947460445625634535952886451045851200930076876761437822444511136389822065421821573050209508437546901404857635498046875", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0xf01dbf291abb8ba3, 0x63db01fcaa7c314b, 0x810d2e304bcb6b22, 0x000000007ff2e341 } };
    Fixed256_str2(&got, "2146624321.504107367316277853651842660823477525064187488105016038871914693839222287383239724865745484758534311363222074699598285090776751693639722832216848119146583584448961801172117702662944793701171875", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x9cb471a55349da48, 0x66fec086df229650, 0x4806aa81e65150b5, 0x0000000004a1bde4 } };
    Fixed256_str2(&got, "77708772.281351715768612519028052816470694074455434306557247689277741095519356493651446205050600942789642100002467064373706602010600692092194967004925817738621163233148081417311914265155792236328125", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0xdb87872d336b1a45, 0xcfa6cf3e53e6d093, 0x903715c8fcaf4a5a, 0x00000000c85f0d46 } };
    Fixed256_str2(&got, "3361672518.563340531891921935596798275963650886297618114122966195666166614823896220299493248199630790644022423946561700922942997598697303119897889562016104518450861082801139900766429491341114044189453125", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x6de2b33b56cef8ec, 0x443baac536891eeb, 0x18ae013eaca91679, 0x00000000d67393d6 } };
    Fixed256_str2(&got, "3597898710.0964051034940534911324067647695659062051650475314890864480939569518959437863396961813049234454749131914496335277023024708185423266426978976274188184829305736656124281580559909343719482421875", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0xc49872c67c081bb7, 0x88534206fc4a447e, 0x10b8fe223c116549, 0x00000000b9b81635 } };
    Fixed256_str2(&got, "3115849269.065322764738146317332007193978584154497800756961494011410183813022940312902620108744740007183317560238752185113612616712785760796502458421657643888153486899650346231283037923276424407958984375", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x220d672b15ad9a9d, 0x2aa3300b2b711343, 0x89c80c4de9367ed9, 0x000000003685156b } };
    Fixed256_str2(&got, "914691435.538208741208302738461095119978276092580745324870360283631303380175049970944910374913087975789234949746109484306133644526325597482081286169891449851676763771468614550030906684696674346923828125", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x550d40ddc2557035, 0x8181e84d99a74924, 0x415ac400d7547080, 0x000000005e3c536c } };
    Fixed256_str2(&got, "1581011820.255291223721842326671251461816037229742658874733871924649972959271585274810341485471866098562381083619492346713134727023516421554022833517013682831241390414067637948392075486481189727783203125", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x1d43d1ffecd1345e, 0xe592067375305db7, 0x1b943cfc46f57327, 0x00000000c979cb06 } };
    Fixed256_str2(&got, "3380202246.10773068579779536629646314421007433599498966480918964444653235317953588698780785715209521681410914278263237200176659937705743434828784932606332241477979649868501610399107448756694793701171875", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x4bb57b5cd3e89d32, 0x9d19ee45032b7328, 0x03b96d91aba018ea, 0x0000000017788b95 } };
    Fixed256_str2(&got, "393776021.01454815679937872136634029845136214988877021211664415408908237369512571867066360691059491789922556951534056116817491917706252053720207338137771285670359067143664333343622274696826934814453125", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0xb62c228e40df7c9a, 0x50806f017a1d556c, 0x35263b4519a2105c, 0x00000000a6ecc31f } };
    Fixed256_str2(&got, "2800534303.20761461674704429437315009683678907521700069797680303037589573295884852368658089772457209271892467870500272105035653428880265356267454871342745489925553370458288782174349762499332427978515625", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x51fbfcc798b8da9f, 0x642a357c732902f4, 0x6607b61550332cb8, 0x00000000101e75eb } };
    Fixed256_str2(&got, "270431723.398555164531171522982999560404687088062440003647747839644374323335986220015492553430496905290897014091557134176545137192990407454901603002055820532275286216528087379629141651093959808349609375", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x513dd1a6e9d40f2b, 0x99f86c8df845aed9, 0x74b31bfbf8449560, 0x000000001c823d9e } };
    Fixed256_str2(&got, "478297502.455857991233886336265154786602475718309847394478210432549888577025009419432861739035747377926288004541142031055088511894523348969810894098659711677341659086248881749270367436110973358154296875", NULL);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x780b25d9b02d3504, 0x5b177a38a96dfb2c, 0x2ee7af97425375be, 0x000000008aa67235 } };
    Fixed256_str2(&got, "2326164021.1832227463040235446664262426495692054686510608867059735264165735487826095881491673592112674881206282864657352269787766400990065575518473964470507497383222617060027914703823626041412353515625", NULL);
    CHECK(got, expect);
  }
}

static void fixed256_test_sqrt(void) {
  union Fixed256 got;
  {
    const union Fixed256 a = { .u64= { 0xfba21a60244cb1f9, 0x78819cd6fbf76c71, 0x1d74fbe3984194ce, 0x10a7c9358e863e37 } };
    const union Fixed256 expect = { .u64= { 0x91b7584a2265b1f5, 0xcd613e30d8f16adf, 0x1027c4d1c386bbc4, 0x00000000414c343c } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x98df57c38ad7b6c2, 0xcaafeff1e7b74182, 0xb6ae6421537d5661, 0x24e43e4cff7be36c } };
    const union Fixed256 expect = { .u64= { 0x7ed4d57b1e2feb89, 0x7311d8a3c2ce6f44, 0xa6cecc1b78e51061, 0x00000000612e7696 } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa2e4c9c8791e9b57, 0x9b5fcff771fb206a, 0xedfb4ed3a4c78dad, 0x0c02a41a7b588184 } };
    const union Fixed256 expect = { .u64= { 0x025b413f8a9a021e, 0xe1988ad9f06c144a, 0xafbd67f9619699cf, 0x0000000037730edf } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x0cf965bc7e88a1c3, 0x1634631d9ea14009, 0x24cb0693ed4b6991, 0x0da506e49568519a } };
    const union Fixed256 expect = { .u64= { 0xf06d3fef701966a0, 0x8d88348a7eed8d14, 0x587fd2803bab6c39, 0x000000003b1a11df } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x256cdb7f0d8bd6e5, 0x16137d79ae5d3059, 0xc10342e3c7ef61a1, 0x167d2891a55cf70c } };
    const union Fixed256 expect = { .u64= { 0x2f978d8719999e3f, 0xfe175330a11d459a, 0xdc2574bdb94067ed, 0x000000004be03db0 } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xddde1daee8d4905d, 0x491c481b69c66947, 0xfb95ce1f21bc3f2c, 0x2bf1a74ce3ec7d46 } };
    const union Fixed256 expect = { .u64= { 0x7af027bc08d6af57, 0xbe6521cc3e2434e3, 0x677f6cbdcc22af58, 0x000000006a107b75 } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xa580a1a05f00171b, 0xbfac6c6ad591c6cd, 0xbfc8a84b302e0384, 0x23253e7556b59422 } };
    const union Fixed256 expect = { .u64= { 0xc74803e31ba16215, 0x855c384429e821a4, 0x64ac5db9d707107e, 0x000000005eda92d8 } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x304da05fc7ecc6f9, 0x882a27e40a2c42f7, 0x6a6c3fa9839dc82b, 0x29e09fb531e74cce } };
    const union Fixed256 expect = { .u64= { 0x8a245e6b33138131, 0xdc3bf364eb8ac8ce, 0x3b6fe5078c5fe8f8, 0x00000000678a5aa3 } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xb749c62a007e93e6, 0xa8210c106897345d, 0xec11984fbbbe4f3e, 0x35f71dbd0d8a4337 } };
    const union Fixed256 expect = { .u64= { 0x5804f92283868a29, 0xd8f33418f3d4e711, 0x5a702cfa93ea5c4e, 0x000000007589a82b } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x53117a504730bdfd, 0xb50a0b99fec3f398, 0x3b75d1d122c235fa, 0x2bc0137842975e2e } };
    const union Fixed256 expect = { .u64= { 0x91eb79fa5d5f576c, 0x3328ad088ded3c96, 0x81355c53f0e642f4, 0x0000000069d495dd } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0xf211ba47093c6698, 0x8503f8152962d90d, 0x75712d8759c89e77, 0x10acb4708db50f5d } };
    const union Fixed256 expect = { .u64= { 0x582c18c92f429ce5, 0x11cbc2884a5012dc, 0x28dd37eb2adf559a, 0x000000004155d7ef } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x02435fa709bc7acf, 0xffe39e12104bbd4f, 0xdccce13957a48f35, 0x03568e98607963b5 } };
    const union Fixed256 expect = { .u64= { 0x7467537a4b63e0ef, 0x526eb523b3df44a4, 0x79490eab7f1a355e, 0x000000001d3b993f } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x8173d416cfb30807, 0xeaa9b751b5ee43d4, 0xdaf924bf01f513ce, 0x090d45e5fe3804ee } };
    const union Fixed256 expect = { .u64= { 0x4fdf8e1a060cea63, 0x57e54acc62f5680c, 0xcbd3f5e06bc15385, 0x000000003023580c } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x5a049d82588add1c, 0xf35bda157bfccf83, 0x489d73bc91c017eb, 0x001c67239779d92a } };
    const union Fixed256 expect = { .u64= { 0xf72f2bb83586fca7, 0x6e80fa489b0bca16, 0xf9bddea5d12982e4, 0x00000000055455e8 } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x31bab5d51c8ed5a1, 0x1559105ab4f48709, 0xd875c50d87f2a83e, 0x27ebf98be3fa64ef } };
    const union Fixed256 expect = { .u64= { 0x736a947a843fdda7, 0x861e02ec39235bc0, 0x07dbf924a6048457, 0x000000006518093d } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const union Fixed256 a = { .u64= { 0x6709e047b0b7a3f6, 0xc890b48d071cd7f5, 0x5cc21fbdabbe2b27, 0x2e85eef822a835c3 } };
    const union Fixed256 expect = { .u64= { 0x936aa40cacc66a57, 0x523d2a54cdaaac43, 0xa185cc8ea8ea37f7, 0x000000006d21f4cd } };
    Fixed256_sqrt(&got, &a);
    CHECK(got, expect);
  }
}

static void fixed256_test_double2(void) {
  union Fixed256 got;
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    Fixed256_double2(&got, -1.164982730250667e-58);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x00000000000000b1, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    Fixed256_double2(&got, 2.8338886449624355e-56);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x000000000000870d, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    Fixed256_double2(&got, 5.507871039208665e-54);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0xffffffffff829869, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff } };
    Fixed256_double2(&got, -1.3092857288243472e-51);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0xfffffffffda9aaf1, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff } };
    Fixed256_double2(&got, -6.246880320155938e-51);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0xffffffe623b15e3a, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff } };
    Fixed256_double2(&got, -1.7694523336555736e-47);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x00004d9d98184b9d, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    Fixed256_double2(&got, 1.3595327992245863e-44);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0093d38c0d6fe640, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 } };
    Fixed256_double2(&got, 6.628763050981942e-42);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x300e5d1f390b2000, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff } };
    Fixed256_double2(&got, -2.3870788069375817e-39);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x838f792c1b300000, 0xffffffffffffff0e, 0xffffffffffffffff, 0xffffffffffffffff } };
    Fixed256_double2(&got, -7.096638424313061e-37);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x6758f2a0a0000000, 0x000000000000abe9, 0x0000000000000000, 0x0000000000000000 } };
    Fixed256_double2(&got, 1.2933201358391382e-34);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x26e0439000000000, 0xffffffffffdd93a5, 0xffffffffffffffff, 0xffffffffffffffff } };
    Fixed256_double2(&got, -6.629678959135302e-33);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x191c500000000000, 0x0000000086499331, 0x0000000000000000, 0x0000000000000000 } };
    Fixed256_double2(&got, 6.62088010461484e-30);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x8690000000000000, 0xffffff01140b6c88, 0xffffffffffffffff, 0xffffffffffffffff } };
    Fixed256_double2(&got, -3.2175642169958808e-27);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x5000000000000000, 0xffffe409ca510b8f, 0xffffffffffffffff, 0xffffffffffffffff } };
    Fixed256_double2(&got, -9.034930854997797e-26);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x00716db1f3511f80, 0x0000000000000000, 0x0000000000000000 } };
    Fixed256_double2(&got, 9.382585326421304e-23);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x75205265d5753000, 0xffffffffffffffff, 0xffffffffffffffff } };
    Fixed256_double2(&got, -2.940765889587178e-20);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0xfa85468ae3c00000, 0x00000000000000e3, 0x0000000000000000 } };
    Fixed256_double2(&got, 1.2358744459870571e-17);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0xe65450c460000000, 0x000000000000cd87, 0x0000000000000000 } };
    Fixed256_double2(&got, 2.852313639404852e-15);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x816db9b000000000, 0xffffffffff0fa97d, 0xffffffffffffffff } };
    Fixed256_double2(&got, -8.538518467802468e-13);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x9229f00000000000, 0xffffffff0d073d69, 0xffffffffffffffff } };
    Fixed256_double2(&got, -2.2098149126700088e-10);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0xd040000000000000, 0x0000001534040096, 0x0000000000000000 } };
    Fixed256_double2(&got, 4.936751460253785e-09);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0xb000000000000000, 0x0000e0d828b86622, 0x0000000000000000 } };
    Fixed256_double2(&got, 1.3401768883621538e-05);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0xffc32d33abef59f0, 0xffffffffffffffff } };
    Fixed256_double2(&got, -0.0009280918930608404);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x37730edf02618800, 0xffffffffffffffff } };
    Fixed256_double2(&got, -0.7834006028693866);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0xb39a197d24400000, 0xffffffffffffffd7 } };
    Fixed256_double2(&got, -40.298429877231456);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x67b28d1d40000000, 0xffffffffffff0ede } };
    Fixed256_double2(&got, -61729.59493177448);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0xb81fb12800000000, 0xffffffffff718191 } };
    Fixed256_double2(&got, -9338478.280766418);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0xab36880000000000, 0xffffffffe032cd7b } };
    Fixed256_double2(&got, -533541508.33119917);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x8e30000000000000, 0xfffffffddb1a2361 } };
    Fixed256_double2(&got, -9208978590.44458);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x6000000000000000, 0xffff7756d8561ff4 } };
    Fixed256_double2(&got, -150260096294923.62);
    CHECK(got, expect);
  }
  {
    const union Fixed256 expect = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xff763423ab517c80 } };
    Fixed256_double2(&got, -3.878621898332659e+16);
    CHECK(got, expect);
  }
}

static void fixed256_test_2double(void) {
  {
    const union Fixed256 a = { .u64= { 0x44cb63e46dd61000, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -2.1490138275225466e-39);
  }
  {
    const union Fixed256 a = { .u64= { 0xe2d5b3584f800000, 0x00000000000000b1, 0x0000000000000000, 0x0000000000000000 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 5.22760185670136e-37);
  }
  {
    const union Fixed256 a = { .u64= { 0x778409f130000000, 0x000000000000870d, 0x0000000000000000, 0x0000000000000000 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 1.0160228745127892e-34);
  }
  {
    const union Fixed256 a = { .u64= { 0x478bfae000000000, 0xffffffffff829868, 0xffffffffffffffff, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -2.4152058758983018e-32);
  }
  {
    const union Fixed256 a = { .u64= { 0xb39bd00000000000, 0xfffffffffda9aaf0, 0xffffffffffffffff, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -1.1523460252500937e-31);
  }
  {
    const union Fixed256 a = { .u64= { 0x4410000000000000, 0xffffffe623b15e39, 0xffffffffffffffff, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -3.264063434957249e-28);
  }
  {
    const union Fixed256 a = { .u64= { 0xa000000000000000, 0x00004d9d98184b9d, 0x0000000000000000, 0x0000000000000000 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 2.5078953607109895e-25);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0093d38c0d6fe640, 0x0000000000000000, 0x0000000000000000 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 1.2227909552672598e-22);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x300e5d1f390b2000, 0xffffffffffffffff, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -4.40338318353535e-20);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x838f792c1b300000, 0xffffffffffffff0e, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -1.3090987279695645e-17);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x6758f2a0a0000000, 0x000000000000abe9, 0x0000000000000000 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 2.3857545551199855e-15);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x26e0439000000000, 0xffffffffffdd93a5, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -1.2229599105002604e-13);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x191c500000000000, 0x0000000086499331, 0x0000000000000000 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 1.2213368083254527e-10);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x8690000000000000, 0xffffff01140b6c88, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -5.935358365164868e-08);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x5000000000000000, 0xffffe409ca510b8f, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -1.666650572058062e-06);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x00716db1f3511f80, 0x0000000000000000 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 0.0017307815026623638);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x3a902932eaba9800, 0xffffffffffffffff } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -0.7712377787295474);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x088803e79e500000, 0x00000000000000e3 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 227.0333254280543);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0xff905d9a30000000, 0x000000000000cd86 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 52614.99829659476);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x7998faf000000000, 0xffffffffff0fa97d } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -15750786.525009457);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x8ba6500000000000, 0xffffffff0d073d69 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -4076389014.4544935);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x45a0000000000000, 0x0000001534040096 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 91066990742.27197);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0xc000000000000000, 0x0000e0d828b86621 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, 247219000731169.75);
  }
  {
    const union Fixed256 a = { .u64= { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xffc32d33abef59f0 } };
    double got = Fixed256_2double(&a);
    CHECK_DOUBLE(got, -1.7120273628177936e+16);
  }
}

