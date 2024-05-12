static void fixed64_test_sqrt(void) {
  Fixed64 got;
  {
    const Fixed64 a = 0x0204f89ac386bbc4;
    const Fixed64 expect = 0x016bcabf2fb14187;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x03c5fd71414c343c;
    const Fixed64 expect = 0x01f1493c5809367f;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0f1ca20c7311d8a3;
    const Fixed64 expect = 0x03e32c6261657157;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0c25ced2a6cecc1b;
    const Fixed64 expect = 0x037c401bec9a9c90;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x06b7f325c9e9c616;
    const Fixed64 expect = 0x02978e72c7b2ec86;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0f9c859018072e8c;
    const Fixed64 expect = 0x03f37d210935ecd2;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0c795051d5f4b3b2;
    const Fixed64 expect = 0x0388268e29d52518;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0885c7a87204e52d;
    const Fixed64 expect = 0x02eb5a38fffb3d44;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x034571e3f1fd42a2;
    const Fixed64 expect = 0x01cf04ab5d7e83c6;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0a286232e6c3f339;
    const Fixed64 expect = 0x032fe73ed32228ef;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x00b6dcdc07d4bedc;
    const Fixed64 expect = 0x00d85ce032ea8c86;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x004b68278a9a021e;
    const Fixed64 expect = 0x008af07bf2fad030;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0b0ffa503bab6c39;
    const Fixed64 expect = 0x03537616869a398d;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0d51589705805975;
    const Fixed64 expect = 0x03a63bd331208b92;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x033333c7a46d6753;
    const Fixed64 expect = 0x01c9f285d9a7781c;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x097c07b6dc2574bd;
    const Fixed64 expect = 0x0314669660285d86;
    got = Fixed64_sqrt(a);
    CHECK(got, expect);
  }
}

static void fixed64_test_mul(void) {
  Fixed64 got;
  {
    const Fixed64 a = 0x0204f89ac386bbc4;
    const Fixed64 b = 0x03c5fd71414c343c;
    const Fixed64 expect = 0x079ebcedc99770d5;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0f1ca20c7311d8a3;
    const Fixed64 b = 0x0c25ced2a6cecc1b;
    const Fixed64 expect = 0xb792f17aed30b596;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x06b7f325c9e9c616;
    const Fixed64 b = 0x0f9c859018072e8c;
    const Fixed64 expect = 0x68e2d8bb489343f2;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0c795051d5f4b3b2;
    const Fixed64 b = 0x0885c7a87204e52d;
    const Fixed64 expect = 0x6a4f43cbce6398f4;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x034571e3f1fd42a2;
    const Fixed64 b = 0x0a286232e6c3f339;
    const Fixed64 expect = 0x213a8def2e1aa926;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x00b6dcdc07d4bedc;
    const Fixed64 b = 0x004b68278a9a021e;
    const Fixed64 expect = 0x0035dd1a6e551885;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0b0ffa503bab6c39;
    const Fixed64 b = 0x0d51589705805975;
    const Fixed64 expect = 0x9353984b1fab5098;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x033333c7a46d6753;
    const Fixed64 b = 0x097c07b6dc2574bd;
    const Fixed64 expect = 0x1e59b7c90d3b6978;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x06133fbeab99254a;
    const Fixed64 b = 0x0917d5624da98f1d;
    const Fixed64 expect = 0x373e08c769a91cf0;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0ffac622e1ea24c4;
    const Fixed64 b = 0x0c965a57815a47c5;
    const Fixed64 expect = 0xc923dd5afbf2ade7;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0f5e04f708d6af57;
    const Fixed64 b = 0x0cefed97cc22af58;
    const Fixed64 expect = 0xc6cf455e59f282cf;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0bbf7a7a2c4a3698;
    const Fixed64 b = 0x02c457af5fec898f;
    const Fixed64 expect = 0x208194d8cda518c3;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x03742c4282283d15;
    const Fixed64 b = 0x053d0434c74803e3;
    const Fixed64 expect = 0x12179a5f5e1b749b;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0bdb525b64ac5db9;
    const Fixed64 b = 0x0f04abad07923986;
    const Fixed64 expect = 0xb2113427e72530fb;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x09df791a0b21fbac;
    const Fixed64 b = 0x05651fde2b9c014e;
    const Fixed64 expect = 0x3543c2eaa1c7d379;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x074312188092b4d4;
    const Fixed64 b = 0x0064c649fb695ffb;
    const Fixed64 expect = 0x02dbd30ad315b639;
    got = Fixed64_mul(a, b);
    CHECK(got, expect);
  }
}

static void fixed64_test_div(void) {
  Fixed64 got;
  {
    const Fixed64 a = 0x02ee433a0e7a269f;
    const Fixed64 b = 0x0b8dc86615ba2bdd;
    const Fixed64 expect = 0x0040efabedeb4c96;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x05692208d5e34124;
    const Fixed64 b = 0x080cf86b4ee207f8;
    const Fixed64 expect = 0x00ac0d4e84a0ee34;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x06ca7f1b9b1f282e;
    const Fixed64 b = 0x0124bc8e9b575bd1;
    const Fixed64 expect = 0x05f0538f320a27f8;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0be7eafdf30b94fa;
    const Fixed64 b = 0x0ba601970706a045;
    const Fixed64 expect = 0x0105a88d4cd33c1d;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0d8e38946148a86f;
    const Fixed64 b = 0x05437d398697bbd0;
    const Fixed64 expect = 0x0293486d1096a2a1;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x05ad8f2f8f7d9b78;
    const Fixed64 b = 0x076118dc3c729578;
    const Fixed64 expect = 0x00c4fa738c6382c3;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x05a7b0a9061b9030;
    const Fixed64 b = 0x058e0a03533c9135;
    const Fixed64 expect = 0x01049e2611a5e50b;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0b829789829e07b0;
    const Fixed64 b = 0x0e42a9cee4c11ab2;
    const Fixed64 expect = 0x00cea0a5b23da824;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0d44fc1bcbf87544;
    const Fixed64 b = 0x0ba7fb30c34c769f;
    const Fixed64 expect = 0x01236ea270bc3718;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0b9492bf5a91c89b;
    const Fixed64 b = 0x0e43bd47f63f23d0;
    const Fixed64 expect = 0x00cfd3c2f81368d2;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0ccbae86c1066932;
    const Fixed64 b = 0x07ff31fe87c56473;
    const Fixed64 expect = 0x01999f04e21e767a;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x08ee67d07d718d73;
    const Fixed64 b = 0x0ff026ebecc1cb63;
    const Fixed64 expect = 0x008f7494a44f59a9;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0e8ccf7fe202849d;
    const Fixed64 b = 0x0b398c167604e4b4;
    const Fixed64 expect = 0x014bd678115eacb4;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0e9c1115b9492f25;
    const Fixed64 b = 0x0f5a3e8be9500ec9;
    const Fixed64 expect = 0x00f39cd42281bd9d;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x09b49b5d4f3f8777;
    const Fixed64 b = 0x0d0371eb96838b76;
    const Fixed64 expect = 0x00beed0c4b640f92;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
  {
    const Fixed64 a = 0x0fa51f2635339774;
    const Fixed64 b = 0x0bbb2c3c830b54fa;
    const Fixed64 expect = 0x015568c875ef2c2f;
    got = Fixed64_div(a, b);
    CHECK(got, expect);
  }
}

static void fixed64_test_2str(void) {
  char buffer[1024];
  {
    const Fixed64 a = 0x0000000000000000;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "0");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "0");
  }
  {
    const Fixed64 a = 0x0100000000000000;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "1");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "1");
  }
  {
    const Fixed64 a = 0xff00000000000000;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "-1");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "-1");
  }
  {
    const Fixed64 a = 0x7fffffffffffffff;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "127.99999999999999998612221219218554324470460414886474609375");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "7f.ffffffffffffff");
  }
  {
    const Fixed64 a = 0x8000000000000000;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "-128");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "-80");
  }
  {
    const Fixed64 a = 0x0204f89ac386bbc4;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "2.019418404327310290202746045906678773462772369384765625");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "2.04f89ac386bbc4");
  }
  {
    const Fixed64 a = 0x03c5fd71414c343c;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "3.773398474160971127755459519903524778783321380615234375");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "3.c5fd71414c343c");
  }
  {
    const Fixed64 a = 0x0f1ca20c7311d8a3;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "15.11184766587561738304668068622049759142100811004638671875");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "f.1ca20c7311d8a3");
  }
  {
    const Fixed64 a = 0x0c25ced2a6cecc1b;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "12.14768711636024571542957772862791898660361766815185546875");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "c.25ced2a6cecc1b");
  }
  {
    const Fixed64 a = 0x06b7f325c9e9c616;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "6.7185538881256195720137469606925151310861110687255859375");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "6.b7f325c9e9c616");
  }
  {
    const Fixed64 a = 0x0f9c859018072e8c;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "15.611413007608627434574799508482101373374462127685546875");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "f.9c859018072e8c");
  }
  {
    const Fixed64 a = 0x0c795051d5f4b3b2;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "12.4738818309167090181421855277221766300499439239501953125");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "c.795051d5f4b3b2");
  }
  {
    const Fixed64 a = 0x0885c7a87204e52d;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "8.52257778915090531690612607462753658182919025421142578125");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "8.85c7a87204e52d");
  }
  {
    const Fixed64 a = 0x034571e3f1fd42a2;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "3.2712690797609505211074321096020867116749286651611328125");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "3.4571e3f1fd42a2");
  }
  {
    const Fixed64 a = 0x0a286232e6c3f339;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "10.15774839528962737722483922198080108501017093658447265625");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "a.286232e6c3f339");
  }
  {
    const Fixed64 a = 0x00b6dcdc07d4bedc;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "0.714307548438906037002738003138802014291286468505859375");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "0.b6dcdc07d4bedc");
  }
  {
    const Fixed64 a = 0x004b68278a9a021e;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "0.2945580209143447703024065731369773857295513153076171875");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "0.4b68278a9a021e");
  }
  {
    const Fixed64 a = 0x0b0ffa503bab6c39;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "11.06241322953012308494269433367662713862955570220947265625");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "b.0ffa503bab6c39");
  }
  {
    const Fixed64 a = 0x0d51589705805975;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "13.31775802501974743929924471785852801986038684844970703125");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "d.51589705805975");
  }
  {
    const Fixed64 a = 0x033333c7a46d6753;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "3.20000884785022536582754781875337357632815837860107421875");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "3.3333c7a46d6753");
  }
  {
    const Fixed64 a = 0x097c07b6dc2574bd;
    Fixed64_2str(buffer, sizeof buffer, a, 10);
    CHECK_STR(buffer, "9.48449271082559426659930323921798844821751117706298828125");
    Fixed64_2str(buffer, sizeof buffer, a, 16);
    CHECK_STR(buffer, "9.7c07b6dc2574bd");
  }
}

static void fixed64_test_str2(void) {
  Fixed64 got;
  {
    const Fixed64 expect = 0x06133fbeab99254a;
    Fixed64_str2(&got, "6.0751914185467700557641279601739370264112949371337890625", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0917d5624da98f1d;
    Fixed64_str2(&got, "9.09309973140767309474075119624103535898029804229736328125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0ffac622e1ea24c4;
    Fixed64_str2(&got, "15.979585819392144563355628861245349980890750885009765625", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0c965a57815a47c5;
    Fixed64_str2(&got, "12.58731600673698299008851364533256855793297290802001953125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0f5e04f708d6af57;
    Fixed64_str2(&got, "15.36726325956140946915606804168419330380856990814208984375", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0cefed97cc22af58;
    Fixed64_str2(&got, "12.93721913083817065537317603229894302785396575927734375", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0bbf7a7a2c4a3698;
    Fixed64_str2(&got, "11.74796260434433248232011237632832489907741546630859375", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x02c457af5fec898f;
    Fixed64_str2(&got, "2.76696296779531379639305299633633694611489772796630859375", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x03742c4282283d15;
    Fixed64_str2(&got, "3.45380035092988564582316968198938411660492420196533203125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x053d0434c74803e3;
    Fixed64_str2(&got, "5.23834543099657383435907576085810433141887187957763671875", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0bdb525b64ac5db9;
    Fixed64_str2(&got, "11.85672541816562998440165443980731652118265628814697265625", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0f04abad07923986;
    Fixed64_str2(&got, "15.0182445662960387389173178007695241831243038177490234375", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x09df791a0b21fbac;
    Fixed64_str2(&got, "9.872941615789371183620204419639776460826396942138671875", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x05651fde2b9c014e;
    Fixed64_str2(&got, "5.3950175148456811224928486581120523624122142791748046875", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x074312188092b4d4;
    Fixed64_str2(&got, "7.261994868650350654082359369567711837589740753173828125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0064c649fb695ffb;
    Fixed64_str2(&got, "0.39365064990977309544728512946676346473395824432373046875", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x06627026c541013d;
    Fixed64_str2(&got, "6.38452379527825981953004230717851896770298480987548828125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0cf14b543b6fe507;
    Fixed64_str2(&got, "12.94255567980862418886456310929133906029164791107177734375", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x0b009f2483868a29;
    Fixed64_str2(&got, "11.00242832385132686623396836012034327723085880279541015625", NULL);
    CHECK(got, expect);
  }
}

static void fixed64_test_double2(void) {
  Fixed64 got;
  {
    const Fixed64 expect = 0x0000000000000000;
    got = Fixed64_double2(-1.0148430870315883e-17);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x00000000000000b1;
    got = Fixed64_double2(2.4686651793873628e-15);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x000000000000870d;
    got = Fixed64_double2(4.798032368428067e-13);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0xffffffffff829869;
    got = Fixed64_double2(-1.1405487277571964e-10);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0xfffffffffda9aaf1;
    got = Fixed64_double2(-5.4418002463091e-10);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0xffffffe623b15e3a;
    got = Fixed64_double2(-1.5414103763202476e-06);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0x00004d9d98184b9d;
    got = Fixed64_double2(0.0011843200993965856);
    CHECK(got, expect);
  }
  {
    const Fixed64 expect = 0xffc9e9c606b7f320;
    got = Fixed64_double2(-0.21127664886448683);
    CHECK(got, expect);
  }
}

static void fixed64_test_2double(void) {
  {
    const Fixed64 a = 0x0204f89ac386bbc4;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 2.01941840432731023469159481464885175228118896484375);
  }
  {
    const Fixed64 a = 0x03c5fd71414c343c;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 3.77339847416097118326661075116135179996490478515625);
  }
  {
    const Fixed64 a = 0x0f1ca20c7311d8a3;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 15.1118476658756168973241074127145111560821533203125);
  }
  {
    const Fixed64 a = 0x0c25ced2a6cecc1b;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 12.1476871163602453407293069176375865936279296875);
  }
  {
    const Fixed64 a = 0x06b7f325c9e9c616;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 6.71855388812561926670241518877446651458740234375);
  }
  {
    const Fixed64 a = 0x0f9c859018072e8c;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 15.6114130076086272680413458147086203098297119140625);
  }
  {
    const Fixed64 a = 0x0c795051d5f4b3b2;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 12.4738818309167083242527951369993388652801513671875);
  }
  {
    const Fixed64 a = 0x0885c7a87204e52d;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 8.522577789150904692405674722976982593536376953125);
  }
  {
    const Fixed64 a = 0x034571e3f1fd42a2;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 3.271269079760950493351856493973173201084136962890625);
  }
  {
    const Fixed64 a = 0x0a286232e6c3f339;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 10.157748395289626586190934176556766033172607421875);
  }
  {
    const Fixed64 a = 0x00b6dcdc07d4bedc;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 0.714307548438906092513889234396629035472869873046875);
  }
  {
    const Fixed64 a = 0x004b68278a9a021e;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 0.294558020914344798057982188765890896320343017578125);
  }
  {
    const Fixed64 a = 0x0b0ffa503bab6c39;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 11.0624132295301222939087892882525920867919921875);
  }
  {
    const Fixed64 a = 0x0d51589705805975;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 13.3177580250197475919549106038175523281097412109375);
  }
  {
    const Fixed64 a = 0x033333c7a46d6753;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 3.200008847850225546238789320341311395168304443359375);
  }
  {
    const Fixed64 a = 0x097c07b6dc2574bd;
    double got = Fixed64_2double(a);
    CHECK_DOUBLE(got, 9.4844927108255934200542469625361263751983642578125);
  }
}

