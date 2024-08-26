	component Tarea1_CPU is
		port (
			btn_export      : in  std_logic_vector(1 downto 0) := (others => 'X'); -- export
			buzzer_export   : out std_logic;                                       -- export
			clk_clk         : in  std_logic                    := 'X';             -- clk
			seg_0_export    : out std_logic_vector(6 downto 0);                    -- export
			seg_1_export    : out std_logic_vector(6 downto 0);                    -- export
			seg_2_export    : out std_logic_vector(6 downto 0);                    -- export
			seg_3_export    : out std_logic_vector(6 downto 0);                    -- export
			switches_export : in  std_logic_vector(3 downto 0) := (others => 'X'); -- export
			leds_export     : out std_logic_vector(4 downto 0)                     -- export
		);
	end component Tarea1_CPU;

	u0 : component Tarea1_CPU
		port map (
			btn_export      => CONNECTED_TO_btn_export,      --      btn.export
			buzzer_export   => CONNECTED_TO_buzzer_export,   --   buzzer.export
			clk_clk         => CONNECTED_TO_clk_clk,         --      clk.clk
			seg_0_export    => CONNECTED_TO_seg_0_export,    --    seg_0.export
			seg_1_export    => CONNECTED_TO_seg_1_export,    --    seg_1.export
			seg_2_export    => CONNECTED_TO_seg_2_export,    --    seg_2.export
			seg_3_export    => CONNECTED_TO_seg_3_export,    --    seg_3.export
			switches_export => CONNECTED_TO_switches_export, -- switches.export
			leds_export     => CONNECTED_TO_leds_export      --     leds.export
		);

